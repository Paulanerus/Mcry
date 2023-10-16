#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <iostream>

namespace Mcry
{
    namespace Threading
    {
        class Medusa
        {
        public:
            Medusa(size_t pool_size);

            ~Medusa();

            template <typename T, typename... Args>
            auto submit(const T &item, Args &&...args)
            {
                std::packaged_task<decltype(item(args...))()> task{std::bind(item, std::forward<Args>(args)...)};

                {
                    std::unique_lock<std::mutex> lock{m_Mutex};
                }

                return task.get_future();
            }

        private:
            const size_t m_PoolSize;

            bool m_IsRunning{true};

            std::condition_variable m_ConditionVariable;

            std::mutex m_Mutex;

            std::vector<std::thread> m_Threads;

            std::queue<std::function<void()>> m_Jobs;
        };
    }
}