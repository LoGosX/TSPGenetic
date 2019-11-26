#pragma once
#include <cstdlib>
#include <vector>
#include "math_helpers.h"

bool betterResultForSwap(std::pair<int,int> a, std::pair<int,int> b, std::pair<int,int> c, std::pair<int,int> d){
    int basic = dist2(a,b) + dist2(c,d);
    int swapped = dist2(a,c) + dist2(b, d);

    return swapped < basic ? true : false;
}

std::vector<int>path2opt(std::vector<int> path, const std::vector<std::pair<int,int>> &cities){
    for(int i=0;i<path.size()-3; i++)
    {
        for(int j=i+2; j<path.size()-1; j++)
        {
            if(betterResultForSwap(cities[path[i]], cities[path[i+1]], cities[path[j]], cities[path[j+1]])){
                int c = path[j];
                path[j] = path[i+1];
                path[i+1] = c;
            }
        }
    }
    return path;
}