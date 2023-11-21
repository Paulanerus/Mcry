#pragma once

#include "socket_utils.hpp"
#include "../pollux/pollux.hpp"

#include <thread>
#include <atomic>

#include <sys/socket.h>

namespace Mcry
{
    class MSocket
    {
    public:
        MSocket(const std::string_view &address_specifier, const SocketConfig &config = {.max_connections = -1, .backlog = 6, .auto_accept = true});

        ~MSocket();

        bool connect() noexcept;

        bool bind();

        bool send(const MBuffer &buffer, int32_t retries = 4);

        MReply recv();

        int32_t accept() const noexcept;

        ConnectionInfo info() const noexcept;

        int32_t descriptor() const noexcept
        {
            return m_Socket;
        }

    private:
        sockaddr_storage getSockAddr(const std::string_view &address, uint16_t port) const noexcept;

    private:
        SocketConfig m_Config;

        int32_t m_Socket;

        ConnectionInfo m_Info{};

        Pollux::PolluxIO m_Pollux{true};

        std::thread m_Accepter;

        std::atomic_bool m_IsRunning{true};
    };
}