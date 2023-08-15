#pragma once 
#include <cmath>
#include <random>

#undef M_PI
#define M_PI 3.14159265358979323846f

extern const float EPSILON;
const float kInfinity = std::numeric_limits<float>::max();

inline float Radians(float deg) { return (M_PI / 180.f) * deg; }
inline float Degrees(float rad) { return (180.f / M_PI) * rad; }

inline float Clamp(float val, float low, float high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}


inline float get_random_float() 
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.f, 1.f);
    return distribution(generator);
}