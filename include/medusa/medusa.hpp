#pragma once

#include <cstddef>

namespace Mcry
{
    namespace Threading
    {
        class Medusa
        {
        public:
            Medusa(size_t pool_size);
            ~Medusa();

        private:
            size_t m_PoolSize;
        };
    }
}