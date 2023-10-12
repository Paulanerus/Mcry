#include "pollux/pollux.hpp"

#include <atomic>
#include <csignal>
#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef TCP_PORT
#define TCP_PORT "8346"
#endif

#ifndef BACKLOG
#define BACKLOG 4
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS 10
#endif

std::atomic_bool is_running{true};

int main(int argc, char *argv[])
{
    std::signal(SIGINT, [](int signal)
                { is_running = false; });

    addrinfo *info{};

    addrinfo hints{};
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("127.0.0.1", TCP_PORT, &hints, &info) != 0)
        return EXIT_FAILURE;

    int sock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (sock == -1)
    {
        std::cout << "Socket error... " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    int32_t opt{1};
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Setsockopt error... " << std::strerror(errno) << std::endl;
        close(sock);
        return EXIT_FAILURE;
    }

    if (bind(sock, info->ai_addr, info->ai_addrlen) == -1)
    {
        std::cout << "Bind error... " << std::strerror(errno) << std::endl;
        close(sock);
        return EXIT_FAILURE;
    }

    if (listen(sock, BACKLOG) == -1)
    {
        std::cout << "Listen error... " << std::strerror(errno) << std::endl;
        close(sock);
        return EXIT_FAILURE;
    }

    std::cout << "Waiting for connection..." << std::endl;

    if (info != nullptr)
        freeaddrinfo(info);

    Mcry::Pollux::PolluxIO pollux{false, -1};

    pollux.add(sock);

    while (is_running)
    {
        Mcry::Pollux::PolluxIOEvent ret = pollux.wait();

        if (ret == Mcry::Pollux::PolluxIOEvent::SUCCESS)
        {
            auto handle_fd = [&pollux, &sock](int32_t file_id)
            {
                if (file_id == sock)
                {
                    int32_t client = accept(sock, nullptr, nullptr);

                    if (client == -1)
                    {
                        std::cout << "Accept error... " << std::strerror(errno) << std::endl;
                        return;
                    }

                    pollux.add(client);
                }
                else
                {
                    char buf[20];
                    std::fill_n(buf, sizeof(buf), 0);

                    int32_t num_bytes = recv(file_id, buf, sizeof(buf), 0);

                    if (num_bytes <= 0)
                    {
                        if (num_bytes == -1)
                            std::cout << std::strerror(errno) << std::endl;

                        close(file_id);

                        pollux.remove(file_id);

                        return;
                    }

                    std::cout << "Client message: " << buf << std::endl;

                    const char *msg = "Hello, client";
                    send(file_id, msg, strlen(msg), 0);
                }
            };

            pollux.handle(handle_fd);
        }
        else if (ret == Mcry::Pollux::PolluxIOEvent::INTERRUPT)
            std::cout << "Interrupted" << std::endl;
        else if (ret == Mcry::Pollux::PolluxIOEvent::TIME_OUT)
            std::cout << "Timed out" << std::endl;
        else
            std::cout << "An error occured (" << std::strerror(errno) << ")" << std::endl;
    }

    std::cout << "\nExiting..." << std::endl;

    close(sock);

    return EXIT_SUCCESS;
}