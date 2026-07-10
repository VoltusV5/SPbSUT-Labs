#include "alg_1.h"
#include <queue>
#include <climits>

void solveDijkstra(int n, const std::vector<std::vector<std::pair<int, int>>>& adj) {
    std::vector<int> dist(n, INT_MAX);
    dist[0] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (const auto& edge : adj[u]) {
            if (dist[u] + edge.second < dist[edge.first]) {
                dist[edge.first] = dist[u] + edge.second;
                pq.push({dist[edge.first], edge.first});
            }
        }
    }
}