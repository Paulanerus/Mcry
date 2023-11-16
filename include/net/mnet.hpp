#pragma once

#include <cstdint>
#include <cstring>

#include <arpa/inet.h>

namespace Mcry
{
    namespace Net
    {
        inline uint64_t HtoNll(uint64_t val)
        {
            static const auto is_network_order = htonl(1) == 1;

            return is_network_order ? val : (static_cast<uint64_t>(htonl(val)) << 32) + htonl(val >> 32);
        }

        inline uint64_t NtoHll(uint64_t val)
        {
            static const auto is_network_order = ntohl(1) == 1;

            return is_network_order ? val : (static_cast<uint64_t>(ntohl(val)) << 32) + ntohl(val >> 32);
        }

        inline float HtoNf(float val)
        {
            static_assert(sizeof(float) == sizeof(uint32_t), "Float is not 32 bits");

            uint32_t tmp = htonl(*reinterpret_cast<uint32_t *>(&val));
            std::memcpy(&val, &tmp, sizeof(float));

            return val;
        }

        inline float NtoHf(float val)
        {
            static_assert(sizeof(float) == sizeof(uint32_t), "Float is not 32 bits");

            uint32_t tmp = ntohl(*reinterpret_cast<uint32_t *>(&val));
            std::memcpy(&val, &tmp, sizeof(float));

            return val;
        }

        inline double HtoNd(double val)
        {
            static_assert(sizeof(double) == sizeof(uint64_t), "Double is not 64 bits");

            uint64_t tmp = HtoNll(*reinterpret_cast<uint64_t *>(&val));
            std::memcpy(&val, &tmp, sizeof(double));

            return val;
        }

        inline double NtoHd(double val)
        {
            static_assert(sizeof(double) == sizeof(uint64_t), "Double is not 64 bits");

            uint64_t tmp = NtoHll(*reinterpret_cast<uint64_t *>(&val));
            std::memcpy(&val, &tmp, sizeof(double));

            return val;
        }
    }
}