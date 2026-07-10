#include "alg_2.h"

void solveBellmanFord(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    std::vector<int> dist(n, 1e9);
    dist[0] = 0;
    for (int i = 0; i < n - 1; i++) {
        for (const auto& edge : edges) {
            int u = std::get<0>(edge);
            int v = std::get<1>(edge);
            int w = std::get<2>(edge);
            if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }
}