#pragma once

#include <cstdint>
#include <string_view>

#include "../buffer/mbuffer.hpp"

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

    struct SocketConfig
    {
        int32_t max_connections;
        uint16_t backlog;
        bool auto_accept;
    };
}