#pragma once

#include <cstddef>

namespace Mcry
{

    class MBuffer
    {
    public:
        const size_t size() noexcept;

    private:
        size_t m_Size;
    };
}