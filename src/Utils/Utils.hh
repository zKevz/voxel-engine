#pragma once
#include <string>
#include <string_view>

namespace Utils
{
    std::string ReadFile(std::string_view path);

    inline constexpr int FloorDiv(int x, int divisor)
    {
        return (x >= 0) ? (x / divisor) : ((x + 1) / divisor - 1);
    }

}  // namespace Utils
