#pragma once
#include <cstdlib>
#include <vector>
#include "math_helpers.h"
#include "TSPOperators.h"

struct TSPLocalSearch
{
    using t_city = std::pair<int,int>;
    bool betterResultForSwap(t_city a, t_city b, t_city c, t_city d)
    {
        float basic = dist(a, b) + dist(c, d);
        float swapped = dist(a, c) + dist(b, d);
        return swapped < basic;
    }

    std::vector<t_city> cities;
    bool full_search;

    TSPLocalSearch(const std::vector<t_city> &cities, bool full_search = false)
    {
        this->full_search = full_search; 
        this->cities = cities;
    }

    

    bool path2opt(std::vector<int> &path)
    {
        auto connections = TSPChromList(path); //with added 0   
        bool improved;
        const int N = path.size() + 1;
        do{
            [&]{
                improved = false;
                TSPChromList::Node* node = connections._nodes[0].get();
                auto before = node->connections[0];
                for(int i = 0; i < N - 1; i++) {
                    auto before_next = node->next(before);
                    auto next = before_next->next(node);
                    for(int j = i + 2; j < N; j++) {
                        if(betterResultForSwap(
                            cities[node->val], cities[node->next(before)->val], 
                            cities[next->val], cities[next->next(before_next)->val]
                            )) {
                                //std::cout << node->val << "->" << node->next(before)->val << ' ' << next->val << "->" << next->next(before_next)->val << '\n';
                                auto node_next = node->next(before);
                                auto next_next = next->next(before_next);
                                node_next->last(node) = next_next;
                                next_next->last(next) = node_next;
                                node->next(before) = next;
                                next->next(before_next) = node;
                                improved = true;
                                return;
                            }
                        auto tmp = next;
                        next = next->next(before_next);
                        before_next = tmp;
                    }
                    auto tmp = node;
                    node = node->next(before);
                    before = tmp;
                }
            }();
        }while(full_search && improved);
        
        path = connections.to_path(path.size());
        return improved;
    }
   
    bool operator()(TSPChrom &chrom)
    {
        return path2opt(chrom.path);
    }
};

bool validate2opt()
{
    for (int i = 0; i < 100; i++)
    {
        auto cities = randomCities(5);
        TSPEvaluator eval(cities);
        TSPLocalSearch s(cities, false);
        TSPChrom chrom;
        chrom.path = randomPath(cities.size());
        float last_path_dist = 100000000000;
        bool improved;
        do
        {
            //std::cout << "1\n";
            improved = s(chrom);
            //std::cout << "2\n";
            float dist = eval.pathDist(chrom);
            if (dist > last_path_dist)
            {
                std::cout << dist << ' ' << last_path_dist << '\n';
                return false;
            }
            //std::cout << last_path_dist - dist << ' ' << improved << '\n';
            last_path_dist = dist;
        }while(improved);
    }
    return true;
}