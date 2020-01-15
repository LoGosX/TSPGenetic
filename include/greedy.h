#include "math_helpers.h"

using t_city = std::pair<int, int>;

int closest(int city, const std::vector<t_city>& cities, const std::vector<std::vector<float>>& distances, const std::vector<bool> visited) {
    int i = -1;
    for(int j = 0; j < cities.size(); j++) {
        if(j != city && !visited[j] && (i == -1 || distances[city][i] > distances[city][j]))
            i = j;
    }
    return i;
}

float greedy(const std::vector<t_city>& cities) {
    const int N = cities.size();
    std::vector<std::vector<float>> distances(N, std::vector<float>(N, 0));
    for(int i = 0; i < N - 1; i++) {
        for(int j = i + 1; j < N; j++) {
            distances[i][j] = distances[j][i] = dist(cities[i], cities[j]);
        }
    }
    float bestDistance = 100000000000;
    for(int i = 0; i < N; i++) {
        std::vector<int> path;
        std::vector<bool> visited(N, false);
        visited[i] = true;
        path.push_back(i);
        for(int j = 0, next = i; j < N - 1; j++) {
            next = closest(next, cities, distances, visited);
            visited[next] = true;
            path.push_back(next);
        }
        float distance = 0;
        for(int k = 1; k < N; k++)
            distance += distances[path[k]][path[k-1]];
        distance += distances[path.back()][path.front()];
        if(distance < bestDistance)
            bestDistance = distance;
    }
    return bestDistance;
}