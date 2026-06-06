#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <cstdint>
#include <unordered_set>
#include <array>
#include <atomic>
#include <bitset>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static inline void check(bool result, const std::string &message = "Check failed")
{
    if (!result)
    {
        throw std::runtime_error(message);
    }
}

#endif // COMMON_HPP