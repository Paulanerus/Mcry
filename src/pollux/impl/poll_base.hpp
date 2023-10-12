#pragma once

#include "pollux/pollux.hpp"

#include <functional>

namespace Mcry
{
    namespace Pollux
    {
        class PollBase
        {
        public:
            PollBase(int32_t timeout = -1) noexcept : m_Timeout(timeout) {}

            virtual void finish() const noexcept {}

            virtual bool add(int32_t file_fd) = 0;

            virtual bool remove(int32_t file_fd) noexcept = 0;

            virtual PolluxIOEvent wait() const noexcept = 0;

            virtual void handle(const std::function<void(int32_t)> &func) const = 0;

            virtual PolluxIOType type() const noexcept = 0;

        protected:
            int32_t m_Timeout;
        };
    }
}
