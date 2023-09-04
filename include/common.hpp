#pragma once 
#include <cmath>
#include <random>
#include <limits>

#undef M_PI
#define M_PI 3.14159265358979323846f

const float MyEPSILON = std::numeric_limits<float>::epsilon();
const float kInfinity = std::numeric_limits<float>::max();

inline float Radians(float deg) { return (M_PI / 180.f) * deg; }
inline float Degrees(float rad) { return (180.f / M_PI) * rad; }

inline float clamp(float val, float low, float high) {
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


class PreGenerated
{
public:
	PreGenerated(size_t size)
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0.0, 1.0);
		vec.resize(size);
		for (size_t i = 0; i < size; ++i)
			vec[i] = dis(gen);
	}
	const std::vector<float>& GetVector() const
	{
		return vec;
	}
private:
	std::vector<float> vec;
};




class RandAccessor
{
public:
	 RandAccessor(size_t offset_, const float* arr_, size_t size_)
		: offset(offset_)
		, arr(arr_)
		, size(size_)
		, index(0)
	{}
	 float Get() const
	{
		size_t i = (offset + index) % size;
		++index;
		return arr[i];
	}
private:
	size_t offset;
	const float* arr;
	size_t size;
	mutable size_t index;
};