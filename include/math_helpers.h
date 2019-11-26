#pragma once
#include <random>

inline std::random_device rd;
inline std::mt19937 g(rd());
inline std::uniform_real_distribution<float> dis(0.0, 1.0);

float randomFloat()
{
    return dis(g);
}

int randomInt(int left, int right) //random integer from range [left, right)
{
    int val = left + (right - left) * randomFloat();
    if(val == right && left != right) //practically impossible BUT ...
        val -= 1;
    return val;
}

template<typename F>
void random_shuffle(F& f)
{
    std::shuffle(f.begin(), f.end(), g);
}

int dist2(std::pair<int,int>& a, std::pair<int,int>& b) {
    return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

float dist(std::pair<int,int>& a, std::pair<int,int>& b) {
    return sqrtf(dist2(a, b));
}   