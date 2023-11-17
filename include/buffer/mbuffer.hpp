#pragma once

#include <cstdint>
#include <string_view>
#include <cstddef>
#include <vector>

namespace Mcry
{
    class MBuffer
    {
    public:
        MBuffer() noexcept = default;

        MBuffer(size_t size);

        size_t size() const noexcept;

        void clear() noexcept;

        bool pop_front(size_t size = 1);

        bool pop_back(size_t size = 1) noexcept;

        MBuffer &put(uint8_t val) noexcept;

        MBuffer &put(int8_t val) noexcept;

        MBuffer &put(uint16_t val) noexcept;

        MBuffer &put(int16_t val) noexcept;

        MBuffer &put(uint32_t val) noexcept;

        MBuffer &put(int32_t val) noexcept;

        MBuffer &put(uint64_t val) noexcept;

        MBuffer &put(int64_t val) noexcept;

        MBuffer &put(float val) noexcept;

        MBuffer &put(double val) noexcept;

        MBuffer &put(bool val) noexcept;

        bool get(uint8_t &val, size_t index = 0) const noexcept;

        bool get(int8_t &val, size_t index = 0) const noexcept;

        bool get(uint16_t &val, size_t index = 0) const noexcept;

        bool get(int16_t &val, size_t index = 0) const noexcept;

        bool get(uint32_t &val, size_t index = 0) const noexcept;

        bool get(int32_t &val, size_t index = 0) const noexcept;

        bool get(uint64_t &val, size_t index = 0) const noexcept;

        bool get(int64_t &val, size_t index = 0) const noexcept;

        bool get(float &val, size_t index = 0) const noexcept;

        bool get(double &val, size_t index = 0) const noexcept;

        bool get(bool &val, size_t index = 0) const noexcept;

        uint8_t operator[](size_t index) const noexcept;

        MBuffer &operator<<(uint8_t val) noexcept;

        MBuffer &operator<<(int8_t val) noexcept;

        MBuffer &operator<<(uint16_t val) noexcept;

        MBuffer &operator<<(int16_t val) noexcept;

        MBuffer &operator<<(uint32_t val) noexcept;

        MBuffer &operator<<(int32_t val) noexcept;

        MBuffer &operator<<(uint64_t val) noexcept;

        MBuffer &operator<<(int64_t val) noexcept;

        MBuffer &operator<<(float val) noexcept;

        MBuffer &operator<<(double val) noexcept;

        MBuffer &operator<<(bool val) noexcept;

        void operator>>(uint8_t &val) const noexcept;

        void operator>>(int8_t &val) const noexcept;

        void operator>>(uint16_t &val) const noexcept;

        void operator>>(int16_t &val) const noexcept;

        void operator>>(uint32_t &val) const noexcept;

        void operator>>(int32_t &val) const noexcept;

        void operator>>(uint64_t &val) const noexcept;

        void operator>>(int64_t &val) const noexcept;

        void operator>>(float &val) const noexcept;

        void operator>>(double &val) const noexcept;

        void operator>>(bool &val) const noexcept;

    private:
        MBuffer &put(uint8_t *data, size_t size) noexcept;

        bool hasEnoughSpace(size_t index, size_t size) const noexcept;

    private:
        mutable std::vector<uint8_t> m_Buffer;
    };
}