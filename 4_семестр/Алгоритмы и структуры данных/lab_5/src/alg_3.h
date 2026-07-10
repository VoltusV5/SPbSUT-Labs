#ifndef BORUVKA_H
#define BORUVKA_H

#include <vector>

// Boruvka's algorithm for Minimum Spanning Tree
// Time complexity: O(E log V)
int boruvkaMST(const std::vector<std::vector<int>>& graph, 
               std::vector<std::pair<int, int>>& mstEdges);

#endif
