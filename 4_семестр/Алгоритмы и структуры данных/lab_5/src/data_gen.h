#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include <vector>
#include <random>
#include <string>

std::vector<std::vector<int>> generateRandomGraph(
    int vertices,
    std::uniform_int_distribution<>& weightDist,
    std::uniform_real_distribution<>& probDist,
    std::mt19937& gen);

void saveGraphToFile(const std::string& filename, 
                     const std::vector<std::vector<int>>& graph);

std::vector<std::vector<int>> loadGraphFromFile(const std::string& filename);

#endif
