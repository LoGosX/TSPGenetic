#pragma once
#include <vector>
#include <algorithm>
#include "math_helpers.h"
#include <iostream>
#include <cmath>
#define DEBUG false 

//cities numbered from 0 !!!
struct TSPChrom
{
    std::vector<int> path;
};

struct TSPChromCreator
{
    int path_length;
    TSPChromCreator(int path_length) : path_length(path_length) {}

    TSPChrom operator()()
    {
        #if DEBUG
        std::cout << "TSPChromCreator::operator()" << std::endl;
        #endif // DEBUG
        TSPChrom chrom;
        chrom.path.resize(path_length - 1);
        return chrom;
    }

    void operator()(TSPChrom& chrom)
    {
        #if DEBUG
        std::cout << "TSPChromCreator::operator(TSPChrom& chrom)" << std::endl;
        #endif // DEBUG
        for(int i = 0; i < path_length - 1; i++)
            chrom.path[i] = i+1;
        random_shuffle(chrom.path);
    }
};

struct TSPCrosser
{
    TSPChrom process(TSPChrom& a, TSPChrom& b, int l, int r)
    {
        #if DEBUG
            std::cout << "TSPCrosser::process(TSPChrom&, TSPChrom&)" << std::endl;
        #endif
        TSPChrom c;
        c.path = b.path;
        std::vector<bool> used(a.path.size(), false);
        for(int i = l; i < r; i++)
            used[b.path[i]] = true;
        int k = r, m = r;
        while(m != l)
        {
            if(!used[a.path[k]])
            {
                used[a.path[k]] = true;
                c.path[m] = a.path[k];
                m = (m + 1) % a.path.size();
            }
            k = (k + 1) % a.path.size();
        }
        return c;
    }
    void operator()(TSPChrom& a, TSPChrom& b)
    {
        #if DEBUG
        std::cout << "TSPCrosser::operator(TSPChrom& a, TSPChrom& b)" << std::endl;
        #endif // DEBUG
        int s = a.path.size();
        int x1 = randomInt(0, s);
        int x2 = randomInt(0, s);
        int l = std::min(x1,x2);
        int r = std::max(x1,x2);
        auto na = process(a,b,l,r);
        auto nb = process(b,a,l,r);
        a = na;
        b = nb;

        #if DEBUG
        assertChromosomes(a,b);
        #endif
    }

    void assertChromosomes(TSPChrom& a, TSPChrom& b)
    {
        std::vector<bool> used(a.path.size());
        for(int i : a.path)
            used[i] = true;
        for(bool x : used)
        {
            if(!x)
                std::cout << "missing a city!\n";
        }
        used = std::vector<bool>(a.path.size());
        for(int i : b.path)
            used[i] = true;
        for(bool x : used)
        {
            if(!x)
                std::cout << "missing a city!\n";
        }
    }
};

struct TSPMutator
{
    void operator()(TSPChrom& chrom)
    {
        #if DEBUG
        std::cout << "TSPMutator::operator(TSPChrom& chrom)" << std::endl;
        #endif // DEBUG
        int x1 = randomInt(0, chrom.path.size());
        int x2 = randomInt(0, chrom.path.size());
        int l = std::min(x1,x2);
        int r = std::max(x1,x2);
        std::reverse(begin(chrom.path) + l, begin(chrom.path) + r + 1);
    }
};

struct TSPEvaluator
{
    std::vector<std::pair<int,int>> cities;
    TSPEvaluator(std::vector<std::pair<int,int>>& cities) : cities(cities) {}

    float pathDist(TSPChrom& chrom)
    {
        float dist = 0;
        dist += sqrtf(dist2(cities[0], cities[chrom.path[0]]));
        for(int i = 1; i < chrom.path.size(); i++)
            dist += sqrtf(dist2(cities[chrom.path[i]], cities[chrom.path[i-1]]));
        dist += sqrtf(dist2(cities[0], cities[chrom.path.back()]));
        return dist;
    }


    float operator()(TSPChrom& chrom)
    {
        #if DEBUG
        std::cout << "TSPMutator::operator(TSPChrom&)" << std::endl;
        #endif // DEBUG
        return 1 / (pathDist(chrom) + 0.0001F);
    }
};