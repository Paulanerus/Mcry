#pragma once

#include <cstdint>
#include <string>
#include <ostream>

#include "../buffer/mbuffer.hpp"

namespace Mcry
{
    enum class Protocol : uint8_t
    {
        TCP = 0,
        UDP = 1,
        RUDP = 2,
    };

    struct ConnectionInfo
    {
        std::string address;
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

std::ostream &operator<<(std::ostream &os, const Mcry::ConnectionInfo &info)
{
    return os << "ConnectionInfo{address: " << info.address << ", port: " << info.port << ", IPv6: " << info.is_ipv6
              << ", Unix: " << info.is_unix << ", Protocol: " << static_cast<uint16_t>(info.protocol) << "}";
}