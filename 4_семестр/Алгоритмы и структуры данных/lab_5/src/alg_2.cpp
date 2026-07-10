#include "alg_2.h"
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

static const int INF_LOCAL = numeric_limits<int>::max() / 2;

int primMST(const vector<vector<int>>& graph, 
            vector<pair<int, int>>& mstEdges) {
    int V = graph.size();
    
    vector<int> key(V, INF_LOCAL);
    vector<bool> inMST(V, false);
    vector<int> parent(V, -1);
    
    key[0] = 0;
    
    int totalWeight = 0;
    mstEdges.clear();
    
    for (int count = 0; count < V; count++) {
        int u = -1;
        int minKey = INF_LOCAL;
        for (int v = 0; v < V; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        inMST[u] = true;
        totalWeight += minKey;
        
        if (parent[u] != -1) {
            mstEdges.push_back({parent[u], u});
        }
        
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != INF_LOCAL && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }
    
    return totalWeight;
}
