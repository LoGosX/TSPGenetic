#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include "math_helpers.h"

float dist(std::pair<int,int>& a, std::pair<int,int>& b)
{
    float x = a.first - b.first;
    float y = a.second - b.second;
    return std::sqrtf(x * x + y * y);
}

std::vector<std::pair<int,int>> randomPath(int size)
{
    std::vector<std::pair<int,int>> v(size);
    for(int i = 0; i < v.size(); i++)
        v[i] = {randomInt(-10000, 10000), randomInt(-10000, 10000)};
    return v;
}

float minimumPathLength(std::vector<std::pair<int,int>> x)
{
    float best = 10000000;
    std::vector<std::pair<int, std::pair<int,int>>> v(x.size());
    for(int i = 0; i < x.size(); i++)
    {
        v[i].first = i;
        v[i].second = x[i];
    }
    do
    {
        float d = 0;
        for(int i = 1; i < v.size(); i++)
        {
            d += dist(v[i].second, v[i-1].second); 
        }
        d += dist(v[0].second, v.back().second);
        best = std::min(best, d);
    }while(std::next_permutation(begin(v), end(v)));
    return best;
}