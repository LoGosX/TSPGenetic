#pragma once
#include <cstdlib>
#include <vector>
#include "math_helpers.h"
#include "TSPOperators.h"

struct TSPLocalSearch
{
    bool betterResultForSwap(std::pair<int, int> a, std::pair<int, int> b, std::pair<int, int> c, std::pair<int, int> d)
    {
        int basic = dist(a, b) + dist(c, d);
        int swapped = dist(a, c) + dist(b, d);

        return swapped < basic;
    }

    std::vector<std::pair<int, int>> cities;
    bool full_search;

    TSPLocalSearch(const std::vector<std::pair<int, int>> &cities, bool full_search)
    {
        this->full_search = full_search; 
        this->cities = cities;
    }

    std::vector<std::vector<int>> toList(std::vector<int>& path){
        std::vector<std::vector<int>> connections(3);
        connections[0].resize((this->cities.size())); //cities on path
        connections[1].resize(cities.size()); //next
        connections[2].resize(cities.size()); //prevoius
        connections[0][0] = 0;
        for(int i = 0; i < path.size(); i++){
            connections[0][i+1] = path[i];
        }
        for(int i = 0; i < cities.size(); i++){
            connections[1][i] = connections[0][(i + 1) % connections[0].size()];
            connections[2][i] = connections[0][(i - 1 + connections[0].size()) % connections[0].size()];
        }
        return connections;
    }

    bool path2opt(std::vector<int> &path)
    {
        auto connections = toList(path); //with added 0   

        bool improved;
        do{
            improved = false;
            for (int i = 0; i < path.size() - 2; i++)
            {
                for (int j = i + 2; j < path.size() - 1; j++)
                {
                    if (betterResultForSwap(cities[path[i]], cities[path[i + 1]], cities[path[j]], cities[path[j + 1]]))
                    {
                         int left = i + 1;
                         int right = j;
                         std::reverse(path.begin() + left, path.begin() + right + 1);
                        improved = true;
                    }
                }
            }
        }while(improved);
        
        return improved;
    }

    /*
    bool path2opt2(std::vector<int> &path)
    {
        auto connections = toList(path); //with added 0   

        bool improved;
        do{
            improved = false;
            for(int i = 0; i < connections[0].size(); i++){
                for(int j = i + 2; j < connections[0].size(); j++){
                    if(betterResultForSwap(cities[connections[0][i]], cities[connections[1][i]], 
                        cities[connections[0][j]], cities[connections[1][j]])){
                            connections[1][i] = connections[0][j]
                        }
                }
            }
        }while(improved);
        
        return improved;
    }
    */

    bool operator()(TSPChrom &chrom)
    {
        return path2opt(chrom.path);
    }
};

bool validate2opt()
{
    for (int i = 0; i < 1000; i++)
    {
        auto cities = randomCities(100);
        TSPEvaluator eval(cities);
        TSPLocalSearch s(cities);
        TSPChrom chrom;
        chrom.path = randomPath(cities.size());
        float last_path_dist = 100000000000;
        for (int i = 0; i < 10; i++)
        {
            s(chrom);
            float dist = eval.pathDist(chrom);
            if (dist > last_path_dist)
            {
                return false;
            }
            last_path_dist = dist;
        }
    }
}