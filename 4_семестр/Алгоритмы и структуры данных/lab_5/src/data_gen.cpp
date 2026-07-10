#include "data_gen.h"
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

static const int INF_LOCAL = numeric_limits<int>::max() / 2;

vector<vector<int>> generateRandomGraph(
    int vertices,
    uniform_int_distribution<>& weightDist,
    uniform_real_distribution<>& probDist,
    mt19937& gen) {
    
    vector<vector<int>> graph(vertices, vector<int>(vertices, INF_LOCAL));
    
    for (int i = 0; i < vertices; i++) {
        graph[i][i] = 0;
    }
    
    const double EDGE_PROBABILITY = 0.25;
    
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (probDist(gen) < EDGE_PROBABILITY) {
                int weight = weightDist(gen);
                graph[i][j] = weight;
                graph[j][i] = weight;
            }
        }
    }
    
    // Ensure connectivity by adding a simple path if necessary (naive way)
    for (int i = 1; i < vertices; i++) {
        if (graph[i][i-1] == INF_LOCAL) {
            int weight = weightDist(gen);
            graph[i][i-1] = weight;
            graph[i-1][i] = weight;
        }
    }
    
    return graph;
}

void saveGraphToFile(const string& filename, 
                     const vector<vector<int>>& graph) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << " for writing" << endl;
        return;
    }
    
    int vertices = graph.size();
    file << vertices << endl;
    
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (graph[i][j] == INF_LOCAL) {
                file << "INF ";
            } else {
                file << graph[i][j] << " ";
            }
        }
        file << endl;
    }
    
    file.close();
}

vector<vector<int>> loadGraphFromFile(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> graph;
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << " for reading" << endl;
        return graph;
    }
    
    int vertices;
    file >> vertices;
    
    graph.resize(vertices, vector<int>(vertices));
    
    string token;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            file >> token;
            if (token == "INF") {
                graph[i][j] = INF_LOCAL;
            } else {
                graph[i][j] = stoi(token);
            }
        }
    }
    
    file.close();
    return graph;
}
