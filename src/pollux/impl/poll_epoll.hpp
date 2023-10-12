#pragma once

#include "poll_base.hpp"

#include <cerrno>

#include <sys/epoll.h>
#include <unistd.h>

namespace Mcry
{
    namespace Pollux
    {
        class PollEPoll : PollBase
        {
        public:
            PollEPoll(int32_t timeout) noexcept : PollBase(timeout), m_EpollFD(epoll_create1(0))
            {
                // TODO handle error
            }

            ~PollEPoll() noexcept override
            {
                close(m_EpollFD);
            }

            bool add(int32_t file_fd) override
            {
                if (m_EpollFD == -1)
                    return false;

                m_Event.data.fd = file_fd;
                return epoll_ctl(m_EpollFD, EPOLL_CTL_ADD, file_fd, &m_Event) != -1;
            }

            bool remove(int32_t file_fd) noexcept override
            {
                return m_EpollFD == -1 ? false : epoll_ctl(m_EpollFD, EPOLL_CTL_ADD, file_fd, nullptr) != -1;
            }

            PolluxIOEvent wait() const noexcept override
            {
                if (m_EpollFD == -1)
                    return PolluxIOEvent::ERROR;

                m_NumEvents = epoll_wait(m_EpollFD, m_EventBatch, s_BatchSize, -1);

                switch (m_NumEvents)
                {
                case -1:
                    if (errno == EINTR)
                        return PolluxIOEvent::INTERRUPT;
                    else
                        return PolluxIOEvent::ERROR;
                case 0:
                    return PolluxIOEvent::TIME_OUT;
                default:
                    return PolluxIOEvent::SUCCESS;
                }
            }

            void handle(const std::function<void(int32_t)> &func) const override
            {
                for (size_t i{}; i < m_NumEvents; i++)
                    func(m_EventBatch[i].data.fd);
            }

            PolluxIOType type() const noexcept override
            {
                return PolluxIOType::EPOLL;
            }

        private:
            static const int32_t s_BatchSize = 12;

            const int32_t m_EpollFD;

            mutable int32_t m_NumEvents{};

            epoll_event m_Event{.events = EPOLL_EVENTS::EPOLLIN};

            mutable epoll_event m_EventBatch[s_BatchSize];
        };
    }
}