#pragma once

#include "../buffer/mbuffer.hpp"

#include <string_view>
#include <cstdint>

#include <sys/socket.h>

namespace Mcry
{
    enum class Protocol
    {
        TCP,
        UDP,
        RUDP,
    };

    struct ConnectionInfo
    {
        std::string_view address;
        uint16_t port;
        bool is_ipv6 = false;
        bool is_unix = false;
        Protocol protocol = Protocol::TCP;
    };

    struct MReply
    {
        bool success;
        MBuffer buffer;
    };

    class MSocket
    {
    public:
        MSocket(const std::string_view &address_specifier);

        ~MSocket();

        bool connect() noexcept;

        bool bind();

        bool send(const MBuffer &buffer, int32_t retries = 4);

        MReply recv();

        ConnectionInfo info() const noexcept;

        int32_t descriptor() const noexcept
        {
            return m_Socket;
        }

    private:
        sockaddr_storage getSockAddr(const std::string_view &address, uint16_t port) const noexcept;

    private:
        int32_t m_Socket;

        ConnectionInfo m_Info{};
    };
}