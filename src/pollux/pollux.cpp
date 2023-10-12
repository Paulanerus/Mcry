#include "pollux/pollux.hpp"

#ifdef __linux__
#define USE_LINUX

#include "impl/poll_poll.hpp"
#include "impl/poll_epoll.hpp"

#elif __FreeBSD__ or __APPLE__
#define USE_BSD
#endif

namespace Mcry
{
    namespace Pollux
    {
        PolluxIO::PolluxIO(bool large, int32_t timeout)
        {
            m_PollBase = std::make_unique<PollPoll>(timeout);
        }

        PolluxIO::~PolluxIO()
        {
            m_PollBase->finish();
        }

        bool PolluxIO::add(int32_t file_fd)
        {
            return m_PollBase->add(file_fd);
        }

        bool PolluxIO::remove(int32_t file_fd) noexcept
        {
            return m_PollBase->remove(file_fd);
        }

        PolluxIOEvent PolluxIO::wait() const noexcept
        {
            return m_PollBase->wait();
        }

        void PolluxIO::handle(const std::function<void(int32_t)> &func) const
        {
            m_PollBase->handle(func);
        }

        PolluxIOType PolluxIO::type() const noexcept
        {
            return m_PollBase->type();
        }
    }
}