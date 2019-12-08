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

std::vector<int> randomPath(int n) {
    std::vector<int> v(n-1);
    for(int i = 0; i < n - 1; i++)
        v[i] = i + 1;
    random_shuffle(v);
    return v;
}

std::vector<std::pair<int,int>> randomCities(int size)
{
    std::vector<std::pair<int,int>> v(size);
    for(int i = 0; i < v.size(); i++)
        v[i] = {randomInt(-10000, 10000), randomInt(-10000, 10000)};
    return v;
}

int dist2(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

float dist(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    return sqrtf(dist2(a, b));
}   