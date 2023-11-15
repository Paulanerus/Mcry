#include "pollux/pollux.hpp"

#include "mcry.hpp"

#include <atomic>
#include <csignal>
#include <cstring>
#include <string>
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

std::atomic_bool is_running{true};

int main(int argc, char *argv[])
{
    std::signal(SIGINT, [](int signal)
                { is_running = false; });

    Mcry::MSocket m_socket{"127.0.0.1:25007/tcp"};

    if (m_socket.bind())
    {
        std::cout << "Failed to bind socket... " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Waiting for connection..." << std::endl;

    Mcry::Pollux::PolluxIO pollux{true};

    std::cout << "Type: " << pollux.type() << std::endl;

    const auto sock = m_socket.descriptor();

    pollux.add(sock);

    while (is_running)
    {
        Mcry::Pollux::PolluxIOEvent ret = pollux.wait();

        if (ret != Mcry::Pollux::PolluxIOEvent::SUCCESS)
        {
            if (ret == Mcry::Pollux::PolluxIOEvent::ERROR)
                std::cout << "Pollux error... " << std::strerror(errno) << std::endl;
            else if (ret == Mcry::Pollux::PolluxIOEvent::TIME_OUT)
                std::cout << "Pollux timeout..." << std::endl;
            else
                std::cout << "Pollux interrupted" << std::endl;

            continue;
        }

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

                const std::string msg{"Hello, client"};
                send(file_id, &msg[0], msg.length(), 0);
            }
        };

        pollux.handle(handle_fd);
    }

    std::cout << "\nExiting..." << std::endl;

    close(sock);

    return EXIT_SUCCESS;
}