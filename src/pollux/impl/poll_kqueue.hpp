#pragma once

#include "poll_base.hpp"

#include <unistd.h>

namespace Mcry
{
    namespace Pollux
    {
        class PollEPoll : public PollBase
        {
        public:
            PollEPoll(int32_t timeout) noexcept : PollBase(timeout)
            {
            }

            ~PollEPoll() noexcept override
            {
            }

            bool add(int32_t file_fd) override
            {
            }

            bool remove(int32_t file_fd) noexcept override
            {
            }

            PolluxIOEvent wait() const noexcept override
            {
            }

            void handle(const std::function<void(int32_t)> &func) const override
            {
            }

            PolluxIOType type() const noexcept override
            {
                return PolluxIOType::KQUEUE;
            }

        private:
        };
    }
}