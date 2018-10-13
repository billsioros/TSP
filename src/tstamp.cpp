
#include "tstamp.hpp"
#include <utility>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <iomanip>

TStamp::TStamp(uint8_t H, uint8_t M)
:
std::pair<uint8_t, uint8_t>(H, M)
{
    if (H > 23)
        throw std::invalid_argument("Hours should be in the range [00, 23]");

    if (M > 59)
        throw std::invalid_argument("Minutes should be in the range [00, 59]");
}

uint32_t TStamp::seconds() const
{
    const uint32_t s1 = static_cast<uint32_t>(first)  * 3600U;
    const uint32_t s2 = static_cast<uint32_t>(second) * 60U;

    return s1 + s2;
}

std::ostream& operator<<(std::ostream& os, const TStamp& tstamp)
{
    os
    << std::setw(2) << std::setfill('0') << static_cast<int>(tstamp.first)
    << ":"
    << std::setw(2) << std::setfill('0') << static_cast<int>(tstamp.second)
    << std::endl;

    return os;
}
