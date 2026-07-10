#ifndef PRIM_H
#define PRIM_H

#include <vector>

// Naive Prim's algorithm for Minimum Spanning Tree
// Time complexity: O(V²) - good for dense graphs
// Returns the total weight of the MST
int primMST(const std::vector<std::vector<int>>& graph, 
            std::vector<std::pair<int, int>>& mstEdges);

#endif
