#include "alg_1.h"
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

static const int INF_LOCAL = numeric_limits<int>::max() / 2;

struct Edge {
    int u, v, weight;
    Edge(int u_, int v_, int w_) : u(u_), v(v_), weight(w_) {}
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DSU {
private:
    vector<int> parent, rank;
public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return false;
        
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
        }
        return true;
    }
};

int kruskalMST(const vector<vector<int>>& graph, 
               vector<pair<int, int>>& mstEdges) {
    int V = graph.size();
    vector<Edge> edges;
    edges.reserve(V * V / 2);
    
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != INF_LOCAL && graph[i][j] > 0) {
                edges.push_back(Edge(i, j, graph[i][j]));
            }
        }
    }
    
    sort(edges.begin(), edges.end());
    
    DSU dsu(V);
    int totalWeight = 0;
    int edgesAdded = 0;
    mstEdges.clear();
    mstEdges.reserve(V - 1);
    
    for (const Edge& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            totalWeight += edge.weight;
            mstEdges.push_back({edge.u, edge.v});
            edgesAdded++;
            if (edgesAdded == V - 1) break;
        }
    }
    
    return totalWeight;
}
