#include "mcry.hpp"

#include <atomic>
#include <csignal>
#include <string>
#include <iostream>

std::atomic_bool is_running{true};

int main(int argc, char *argv[])
{
    std::signal(SIGINT, [](int signal)
                { is_running = false; });

    Mcry::MSocket m_socket{"127.0.0.1:25007/tcp"};

    if (!m_socket.bind())
    {
        std::cout << "Failed to bind socket... " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Waiting for connection..." << std::endl;

    Mcry::MBuffer buffer;

    buffer << std::string{"Hello, Client"};

    Mcry::Pollux::PolluxIO pollux{true};

    const auto sock = m_socket.descriptor();

    pollux.add(sock);

    while (is_running)
    {
        Mcry::Pollux::PolluxIOEvent ret = pollux.wait();

        if (ret != Mcry::Pollux::PolluxIOEvent::SUCCESS)
            continue;

        auto handle_fd = [&](int32_t file_id)
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
                char buf[20] = {};

                int32_t num_bytes = recv(file_id, buf, sizeof(buf), 0);
                // auto [succeded, recv_buf] = m_socket.recv();

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

                // m_socket.send(buffer);
            }
        };

        pollux.handle(handle_fd);
    }

    std::cout << "\nExiting..." << std::endl;

    return EXIT_SUCCESS;
}