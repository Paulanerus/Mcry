#include "medusa/medusa.hpp"

#include <iostream>

namespace Mcry
{
    namespace Threading
    {
        Medusa::Medusa(size_t pool_size) : m_PoolSize{pool_size}
        {
            auto worker = [this]
            {
                while (!m_IsRunning)
                {
                    std::unique_lock<std::mutex> lock{m_Mutex};
                    m_ConditionVariable.wait(lock, [this]
                                             { return !m_Jobs.empty() || !m_IsRunning; });

                }
            };

            for (size_t i{}; i < pool_size; i++)
                m_Threads.emplace_back(worker);
        }

        Medusa::~Medusa()
        {
            m_IsRunning = false;

            m_ConditionVariable.notify_all();

            for (auto &thread : m_Threads)
                thread.join();
        }

    }
}