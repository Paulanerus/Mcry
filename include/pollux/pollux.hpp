#pragma once

#include <memory>
#include <functional>

namespace Mcry
{
    namespace Pollux
    {
        class PollBase;

        enum PolluxIOType
        {
            POLL,
            EPOLL,
            KQUEUE,
        };

        enum PolluxIOEvent
        {
            SUCCESS,
            TIME_OUT,
            INTERRUPT,
            ERROR,
        };

        class PolluxIO
        {

        public:
            PolluxIO(bool large, int32_t timeout);

            ~PolluxIO();

            bool add(int32_t file_fd);

            bool remove(int32_t file_fd) noexcept;

            PolluxIOEvent wait() const noexcept;

            void handle(const std::function<void(int32_t)> &func) const;

            PolluxIOType type() const noexcept;

        private:
            std::unique_ptr<PollBase> m_PollBase;
        };
    }
}
