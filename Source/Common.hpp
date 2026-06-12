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
#include <random>
#include <cmath>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>

#define USE_ACTORS 0

static inline void check(bool result, const std::string &message = "Check failed")
{
    if (!result)
    {
        throw std::runtime_error(message);
    }
}

static inline float RandomFloat(float min, float max)
{
    static std::random_device randomDevice;
    static std::mt19937 rng(randomDevice());

    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

#endif // COMMON_HPP