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

#include <SDL3/SDL.h>

static inline void check(bool result, const std::string &message = "Check failed")
{
    if (!result)
    {
        throw std::runtime_error(message);
    }
}

#endif // COMMON_HPP