#pragma once
#include <cstdlib>
#include <vector>

using namespace std;

struct node{
    int x,y;
    bool used;
};

int distances2[1001][1001];
int distance2(const node&, const node&);

void calculateDistances(const vector<node>& data)
{
    for(int i = 1; i < data.size(); i++)
        for(int j = i; j < data.size(); j++)
            distances2[i][j] = distances2[j][i] = distance2(data[i], data[j]);
}

int distance2(const node& A, const node& B){
    int x = A.x - B.x;
    int y = A.y - B.y;
    return x * x + y * y;
}

int closestNodeIndex(vector<node>& data, int starting_node_index){
    
    int closest_node_index = -1;
    
    for(int i = 1; i < data.size(); i++){
        if(!data[i].used)
            if(closest_node_index == -1 || distances2[i][starting_node_index] < distances2[closest_node_index][starting_node_index])
                closest_node_index = i;
    }

    return closest_node_index;
}

vector<int> naiwnie(vector<node>& data){
    vector<int> kolejnosc = {1};
    data[1].used = true;
    for(int i = 1; i < data.size() - 1; i++){
        int closest_node_index = closestNodeIndex(data, kolejnosc.back());
        data[closest_node_index].used = true;
        kolejnosc.push_back(closest_node_index);  
    } 
    return kolejnosc;
}


std::vector<int> pathGreedy(const std::vector<std::pair<int,int>>& cities)
{
    std::vector<node> nodes;
    for(auto city : cities)
    {
        nodes.push_back({city.first, city.second, false});
    }

    return naiwnie(nodes);
}