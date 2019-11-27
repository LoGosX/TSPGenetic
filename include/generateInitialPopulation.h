#pragma once
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "math_helpers.h"



int getClosestCity(std::vector<int> path , std::vector<std::pair<int,int>> cities)
{
    int currMin = -1;
    for(int i = 0 ; i < cities.szie(); i++){
        if(currMin >= 0){
            
        }else if (currMin >=0){
            for(int i = 0; i < cities.size(); i++)
            {
                
            }
        }
    }
}

//generate greedy path with result[0] = n
std::vector<int> greedyPathFromN (std::vector<std::pair<int,int>> cities, int n)
{
    std::vector<int> path(n);
    path.push_back(n);

    for(int i=1; i<cities.size();i++){
        path.push_back(getClosestCity())
    }

    return path;
}

//generate n greedy paths from random initial cities
std::vector<std::vector<int>>generateGreedyPopulation(const std::vector<std::pair<int,int>> cities, int n)
{
    std::vector<std::vector<int>> greedyPaths(n);
    for(int i = 0; i < n; i++)
    {
        int randomFirstCity = randomInt(0, cities.size());
        greedyPaths.push_back(greedyPathFromN(cities, randomFirstCity));
    }
    return greedyPaths;
}