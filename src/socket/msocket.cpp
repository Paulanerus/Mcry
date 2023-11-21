#include "socket/msocket.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

namespace Mcry
{
    MSocket::MSocket(const std::string_view &address_specifier, const SocketConfig &config) : m_Config(config)
    {
        auto ends_with = [](const std::string_view &word, const std::string_view &suffix)
        {
            return word.size() >= suffix.size() && word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0;
        };

        m_Info.protocol = ends_with(address_specifier, "/udp") ? Protocol::UDP : ends_with(address_specifier, "/rudp") ? Protocol::RUDP
                                                                                                                       : Protocol::TCP;

        auto colon_count = std::count(address_specifier.begin(), address_specifier.end(), ':');

        m_Info.is_unix = colon_count == 0;

        m_Info.is_ipv6 = colon_count > 1;

        auto protocol_start = address_specifier.find_last_of("/");
        bool has_protocol = ends_with(address_specifier, "/udp") || ends_with(address_specifier, "/rudp") || ends_with(address_specifier, "/tcp");

        if (m_Info.is_unix)
            m_Info.address = address_specifier.substr(0, (protocol_start == std::string_view::npos || !has_protocol) ? address_specifier.length() : protocol_start);
        else
        {
            auto last_colon = address_specifier.find_last_of(":");

            m_Info.address = address_specifier.substr(0, last_colon);

            auto port_length = std::min<uint8_t>((has_protocol ? protocol_start : address_specifier.length()) - (last_colon + 1), 5);

            char port[6] = {};
            std::copy_n(address_specifier.begin() + last_colon + 1, port_length, port);

            m_Info.port = static_cast<uint16_t>(std::atoi(port));
        }
    }

    MSocket::~MSocket()
    {
        m_IsRunning = false;

        shutdown(m_Socket, SHUT_RDWR);

        if (m_Accepter.joinable())
            m_Accepter.join();

        close(m_Socket);
    }

    bool MSocket::connect() noexcept
    {
        m_Socket = socket(m_Info.is_unix ? AF_UNIX : m_Info.is_ipv6 ? AF_INET6
                                                                    : AF_INET,
                          m_Info.protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM,
                          m_Info.protocol == Protocol::TCP ? IPPROTO_TCP : IPPROTO_UDP);

        if (m_Socket == -1)
            return false;

        // TODO (paul) bind if unix domain socket

        auto addr = getSockAddr(m_Info.address, m_Info.port);

        return ::connect(m_Socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != -1;
    }

    bool MSocket::bind()
    {
        m_Socket = socket(m_Info.is_unix ? AF_UNIX : m_Info.is_ipv6 ? AF_INET6
                                                                    : AF_INET,
                          m_Info.protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM,
                          m_Info.protocol == Protocol::TCP ? IPPROTO_TCP : IPPROTO_UDP);

        if (m_Socket == -1)
            return false;

        auto addr = getSockAddr(m_Info.address, m_Info.port);

        if (::bind(m_Socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
            return false;

        if (m_Info.protocol == Protocol::TCP && listen(m_Socket, m_Config.backlog) == -1)
            return false;

        if (!m_Config.auto_accept || m_Info.protocol != Protocol::TCP)
            return true;

        std::cout << "Starting accepter thread..." << std::endl;

        auto accept_lamba = [this]
        {
            while (m_IsRunning)
            {
                auto client_fd = ::accept(m_Socket, nullptr, nullptr);

                if (client_fd == -1)
                    continue;

                if (!m_Pollux.add(client_fd))
                    std::cout << "Failed to add client file descriptor" << std::endl;
            }
        };

        try
        {
            m_Accepter = std::thread{accept_lamba};
            return true;
        }
        catch (std::system_error &exception)
        {
            m_Config.auto_accept = false;

            std::cout << "Failed to start accepter thread: " << exception.what() << " (" << exception.code() << ")" << std::endl;

            return false;
        }
    }

    bool MSocket::send(const MBuffer &buffer, int32_t retries)
    {
        // TODO (paul) implementation

        return false;
    }

    MReply MSocket::recv()
    {
        // TODO (paul) implementation

        return MReply{};
    }

    int32_t MSocket::accept() const noexcept
    {
        return (m_Config.auto_accept || m_Info.protocol != Protocol::TCP) ? -1 : ::accept(m_Socket, nullptr, nullptr);
    }

    ConnectionInfo MSocket::info() const noexcept
    {
        return m_Info;
    }

    sockaddr_storage MSocket::getSockAddr(const std::string_view &address, uint16_t port) const noexcept
    {
        // TODO (paul) error handling

        sockaddr_storage addr{};

        if (m_Info.is_unix)
        {
            auto addr_un = reinterpret_cast<sockaddr_un *>(&addr);

            addr_un->sun_family = AF_UNIX;
            strncpy(addr_un->sun_path, address.data(), sizeof(addr_un->sun_path) - 1);
        }
        else if (m_Info.is_ipv6)
        {
            auto addr_in6 = reinterpret_cast<sockaddr_in6 *>(&addr);

            addr_in6->sin6_family = AF_INET6;
            addr_in6->sin6_port = htons(port);
            inet_pton(AF_INET6, address.data(), &addr_in6->sin6_addr);
        }
        else
        {
            auto addr_in = reinterpret_cast<sockaddr_in *>(&addr);

            addr_in->sin_family = AF_INET;
            addr_in->sin_port = htons(port);
            inet_pton(AF_INET, address.data(), &addr_in->sin_addr);
        }

        return addr;
    }
}