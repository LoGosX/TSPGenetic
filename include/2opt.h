#pragma once
#include <cstdlib>
#include <vector>
#include "math_helpers.h"
#include "TSPOperators.h"

struct TSPLocalSearch
{
    bool betterResultForSwap(std::pair<int,int> a, std::pair<int,int> b, std::pair<int,int> c, std::pair<int,int> d){
        int basic = dist2(a,b) + dist2(c,d);
        int swapped = dist2(a,c) + dist2(b, d);

        return swapped < basic;
    }

    std::vector<std::pair<int,int>> cities;

    TSPLocalSearch(const std::vector<std::pair<int,int>>& cities) {
        this->cities = cities;
    }

    void path2opt(std::vector<int> &path){
        for(int j=1; j<path.size(); j++)
        {
            if(betterResultForSwap(cities[0], cities[path[1]], cities[path[j]], cities[path[(j+1)%path.size()]])){
                std::swap(path[j], path[1]);
            }
        }
        for(int i=0;i<path.size()-3; i++)
        {
            for(int j=i+2; j<path.size(); j++)
            {
                if(betterResultForSwap(cities[path[i]], cities[path[i+1]], cities[path[j]], cities[path[(j+1)%path.size()]])){
                    std::swap(path[j], path[i+1]);
                }
            }
        }
    }

    void operator()(TSPChrom& chrom)
    {
        path2opt(chrom.path);
    }
};