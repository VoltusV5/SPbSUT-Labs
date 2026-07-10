#include "alg_3.h"
#include <vector>
#include <limits>
#include <functional>

using namespace std;

static const int INF_LOCAL = numeric_limits<int>::max() / 2;

int boruvkaMST(const vector<vector<int>>& graph, 
               vector<pair<int, int>>& mstEdges) {
    int V = graph.size();
    vector<int> parent(V);
    vector<int> rank_(V, 0);
    
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }
    
    function<int(int)> find = [&](int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };
    
    auto unite = [&](int x, int y) -> bool {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (rank_[x] < rank_[y]) swap(x, y);
        parent[y] = x;
        if (rank_[x] == rank_[y]) rank_[x]++;
        return true;
    };
    
    int numTrees = V;
    int mstWeight = 0;
    mstEdges.clear();
    
    while (numTrees > 1) {
        vector<int> cheapest(V, -1);
        vector<int> cheapestWeight(V, INF_LOCAL);
        vector<int> cheapestTarget(V, -1);
        
        bool foundAny = false;
        for (int u = 0; u < V; u++) {
            int setU = find(u);
            
            for (int v = 0; v < V; v++) {
                if (u == v || graph[u][v] >= INF_LOCAL || graph[u][v] <= 0) continue;
                
                int setV = find(v);
                if (setU == setV) continue;
                
                if (graph[u][v] < cheapestWeight[setU]) {
                    cheapestWeight[setU] = graph[u][v];
                    cheapest[setU] = u;
                    cheapestTarget[setU] = v;
                    foundAny = true;
                }
            }
        }
        
        if (!foundAny) break; // Graph is not connected
        
        for (int i = 0; i < V; i++) {
            if (cheapest[i] != -1) {
                int u = cheapest[i];
                int v = cheapestTarget[i];
                if (unite(u, v)) {
                    mstWeight += graph[u][v];
                    mstEdges.push_back({u, v});
                    numTrees--;
                }
            }
        }
    }
    
    return mstWeight;
}
