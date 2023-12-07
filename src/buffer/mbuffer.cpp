#include "buffer/mbuffer.hpp"

#include "net/mnet.hpp"

#include <iostream>

namespace Mcry
{
    MBuffer::MBuffer(size_t size)
    {
        m_Buffer.reserve(size);
    }

    size_t MBuffer::size() const noexcept
    {
        return m_Buffer.size();
    }

    void MBuffer::clear() noexcept
    {
        m_Buffer.clear();
    }

    uint8_t *MBuffer::data() const noexcept
    {
        return &m_Buffer[0];
    }

    bool MBuffer::popFront(size_t size)
    {
        if (!hasEnoughSpace(0, size))
            return false;

        m_Buffer.erase(m_Buffer.begin(), m_Buffer.begin() + size);

        return true;
    }

    bool MBuffer::popBack(size_t size) noexcept
    {
        if (!hasEnoughSpace(0, size))
            return false;

        for (size_t i{}; i < size; i++)
            m_Buffer.pop_back();

        return true;
    }

    std::string MBuffer::toString() const
    {
        return std::string{m_Buffer.begin(), m_Buffer.end()};
    }

    MBuffer &MBuffer::put(uint8_t val) noexcept
    {
        return put(&val, sizeof(uint8_t));
    }

    MBuffer &MBuffer::put(int8_t val) noexcept
    {
        return put(reinterpret_cast<uint8_t *>(&val), sizeof(int8_t));
    }

    MBuffer &MBuffer::put(uint16_t val) noexcept
    {
        auto val_no = htons(val);

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(uint16_t));
    }

    MBuffer &MBuffer::put(int16_t val) noexcept
    {
        auto val_no = static_cast<int16_t>(htons(val));

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(int16_t));
    }

    MBuffer &MBuffer::put(uint32_t val) noexcept
    {
        auto val_no = htonl(val);

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(uint32_t));
    }

    MBuffer &MBuffer::put(int32_t val) noexcept
    {
        auto val_no = static_cast<int32_t>(htonl(val));

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(int32_t));
    }

    MBuffer &MBuffer::put(uint64_t val) noexcept
    {
        auto val_no = Net::HtoNll(val);

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(uint64_t));
    }

    MBuffer &MBuffer::put(int64_t val) noexcept
    {
        auto val_no = static_cast<int64_t>(Net::HtoNll(val));

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(int64_t));
    }

    MBuffer &MBuffer::put(float val) noexcept
    {
        auto val_no = Net::HtoNf(val);

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(float));
    }

    MBuffer &MBuffer::put(double val) noexcept
    {
        auto val_no = Net::HtoNd(val);

        return put(reinterpret_cast<uint8_t *>(&val_no), sizeof(double));
    }

    MBuffer &MBuffer::put(bool val) noexcept
    {
        return put(reinterpret_cast<uint8_t *>(&val), sizeof(bool));
    }

    MBuffer &MBuffer::put(const std::string &val) noexcept
    {
        return put(reinterpret_cast<uint8_t *>(&(const_cast<std::string &>(val)[0])), val.length());
    }

    MBuffer &MBuffer::put(const char *val) noexcept
    {
        return put(reinterpret_cast<uint8_t *>(const_cast<char *>(val)), std::char_traits<char>::length(val));
    }

    bool MBuffer::get(uint8_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(uint8_t)))
            return false;

        val = m_Buffer[index];

        return true;
    }

    bool MBuffer::get(int8_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(int8_t)))
            return false;

        val = static_cast<int8_t>(m_Buffer[index]);

        return true;
    }

    bool MBuffer::get(uint16_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(uint16_t)))
            return false;

        uint16_t temp = *reinterpret_cast<uint16_t *>(&m_Buffer[index]);
        val = ntohs(temp);

        return true;
    }

    bool MBuffer::get(int16_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(int16_t)))
            return false;

        int16_t temp = *reinterpret_cast<int16_t *>(&m_Buffer[index]);
        val = static_cast<int16_t>(ntohs(temp));

        return true;
    }

    bool MBuffer::get(uint32_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(uint32_t)))
            return false;

        uint32_t temp = *reinterpret_cast<uint32_t *>(&m_Buffer[index]);
        val = ntohl(temp);

        return true;
    }

    bool MBuffer::get(int32_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(int32_t)))
            return false;

        int32_t temp = *reinterpret_cast<int32_t *>(&m_Buffer[index]);
        val = static_cast<int32_t>(ntohl(temp));

        return true;
    }

    bool MBuffer::get(uint64_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(uint64_t)))
            return false;

        uint64_t temp = *reinterpret_cast<uint64_t *>(&m_Buffer[index]);
        val = Net::NtoHll(temp);

        return true;
    }

    bool MBuffer::get(int64_t &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(int64_t)))
            return false;

        int64_t temp = *reinterpret_cast<int64_t *>(&m_Buffer[index]);
        val = static_cast<int64_t>(Net::NtoHll(temp));

        return true;
    }

    bool MBuffer::get(float &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(float)))
            return false;

        float temp = *reinterpret_cast<float *>(&m_Buffer[index]);
        val = Net::NtoHf(temp);

        return true;
    }

    bool MBuffer::get(double &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(double)))
            return false;

        double temp = *reinterpret_cast<double *>(&m_Buffer[index]);
        val = Net::NtoHd(temp);

        return true;
    }

    bool MBuffer::get(bool &val, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, sizeof(bool)))
            return false;

        val = static_cast<bool>(m_Buffer[index]);

        return true;
    }

    bool MBuffer::get(std::string &val, size_t length, size_t index) const noexcept
    {
        if (!hasEnoughSpace(index, length))
            return false;

        for (size_t i{}; i < length; i++)
            val += m_Buffer[index + i];

        return true;
    }

    uint8_t MBuffer::operator[](size_t index) const noexcept
    {
        if (index < 0 || index >= m_Buffer.size())
            return 0;

        return m_Buffer[index];
    }

    MBuffer &MBuffer::operator<<(uint8_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(int8_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(uint16_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(int16_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(uint32_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(int32_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(uint64_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(int64_t val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(float val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(double val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(bool val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(const std::string &val) noexcept
    {
        return put(val);
    }

    MBuffer &MBuffer::operator<<(const char *val) noexcept
    {
        return put(val);
    }

    void MBuffer::operator>>(uint8_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(int8_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(uint16_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(int16_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(uint32_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(int32_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(uint64_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(int64_t &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(float &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(double &val) const noexcept
    {
        get(val);
    }

    void MBuffer::operator>>(bool &val) const noexcept
    {
        get(val);
    }

    bool MBuffer::operator==(const MBuffer &other) const noexcept
    {
        return m_Buffer == other.m_Buffer;
    }

    bool MBuffer::hasEnoughSpace(size_t index, size_t size) const noexcept
    {
        if (index < 0 || index >= m_Buffer.size())
            return false;

        return m_Buffer.size() - index >= size;
    }

    MBuffer &MBuffer::put(uint8_t *data, size_t size) noexcept
    {
        for (size_t i{}; i < size; i++)
            m_Buffer.push_back(data[i]);

        return *this;
    }
}