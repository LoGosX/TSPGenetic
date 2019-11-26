#pragma once
#include <cstdlib>
#include <random>

float randomFloat()
{
    return (float) rand() / RAND_MAX;
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
    static std::random_device rd;
    static std::mt19937 g(rd());

    std::shuffle(f.begin(), f.end(), g);
}

int dist2(std::pair<int,int>& a, std::pair<int,int>& b) {
    return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

float dist(std::pair<int,int>& a, std::pair<int,int>& b) {
    return sqrtf(dist2(a, b));
}   