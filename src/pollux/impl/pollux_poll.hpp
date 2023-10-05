#pragma once

#include "pollux_base.hpp"

#include <vector>

#include <poll.h>

namespace Mcry
{
    namespace Pollux
    {
        class PolluxIOPoll : public PolluxIOBase
        {
        public:
            PolluxIOPoll(int32_t timeout) noexcept : PolluxIOBase(timeout)
            {
            }

            bool add(int32_t file_fd) override
            {
                for (const auto &entry : m_FileDescriptors)
                {
                    if (entry.fd == file_fd)
                        return false;
                }

                m_FileDescriptors.emplace_back(pollfd{file_fd, POLLIN, 0});

                return true;
            }

            bool remove(int32_t file_fd) noexcept override
            {
                size_t idx{};
                bool found{};
                for (; idx < m_FileDescriptors.size(); idx++)
                {
                    if (m_FileDescriptors[idx].fd == file_fd)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    m_FileDescriptors.erase(m_FileDescriptors.begin() + idx);

                return found;
            }

            PolluxIOEvent wait() const noexcept override
            {
                int32_t ret = poll(&m_FileDescriptors[0], m_FileDescriptors.size(), m_Timeout);

                switch (ret)
                {
                case -1:
                    return PolluxIOEvent::ERROR;
                case 0:
                    return PolluxIOEvent::TIME_OUT;
                default:
                    return PolluxIOEvent::SUCCESS;
                }
            }

            void handle(const std::function<void(int32_t)> &func) const override
            {
                for (size_t i{}; i < m_FileDescriptors.size(); i++)
                {
                    if (m_FileDescriptors[i].revents & POLLIN)
                        func(m_FileDescriptors[i].fd);
                }
            }

            PolluxIOType type() const noexcept override
            {
                return PolluxIOType::POLL;
            }

        private:
            mutable std::vector<pollfd> m_FileDescriptors;
        };
    }
}