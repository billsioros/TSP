
#pragma once

#include <utility>
#include <cstdint>
#include <iosfwd>

struct TStamp : public std::pair<uint8_t, uint8_t>
{
    TStamp(uint8_t, uint8_t);

    uint32_t seconds() const;

    friend std::ostream& operator<<(std::ostream&, const TStamp&);
};
