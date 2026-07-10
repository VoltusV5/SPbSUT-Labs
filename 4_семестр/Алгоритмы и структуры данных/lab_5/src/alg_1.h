#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>

// Kruskal's algorithm for Minimum Spanning Tree
// Time complexity: O(E·log E)
// Returns the total weight of the MST
int kruskalMST(const std::vector<std::vector<int>>& graph, 
               std::vector<std::pair<int, int>>& mstEdges);

#endif
