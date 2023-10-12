#include "medusa/medusa.hpp"

#include <iostream>

namespace Mcry
{
    namespace Threading
    {
        Medusa::Medusa(size_t pool_size) : m_PoolSize{pool_size}
        {
        }

        Medusa::~Medusa()
        {
        }
    }
}