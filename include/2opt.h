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

    TSPLocalSearch(const std::vector<std::pair<int, int>> &cities)
    {
        this->cities = cities;
    }

    bool path2opt(std::vector<int> &path)
    {
        auto out = [&path]{
            std::cout << "---------------\n";
            for (int x : path)
            {
                if (x == 0)
                    std::cout << '!';
                std::cout << x << ' ';
            }
            std::cout << std::endl;
        };
        path.insert(path.begin(), 0);
        path.push_back(0);
        //out();
        bool improved = false;
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
        //out();
        path.pop_back();
        path.erase(path.begin());
        //out();
        return improved;
    }

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