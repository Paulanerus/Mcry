#include "socket/msocket.hpp"

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

namespace Mcry
{

    MSocket::MSocket(const std::string_view &address_specifier, const SocketConfig &config) : m_Config(config)
    {
        /*
            IP and Port are required otherwise unix domain socket is assumed.
            Protocol is optional and defaults to tcp.

            127.0.0.1:25007\tcp
        */

        m_Info.address = "127.0.0.1";
        m_Info.port = 8346;
    }

    MSocket::~MSocket()
    {
        shutdown(m_Socket, SHUT_RDWR);

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

        if (listen(m_Socket, m_Config.backlog) == -1)
            return false;

        if (!m_Config.auto_accept)
            return true;

        // TODO (paul) thread auto accepter

        return false;
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