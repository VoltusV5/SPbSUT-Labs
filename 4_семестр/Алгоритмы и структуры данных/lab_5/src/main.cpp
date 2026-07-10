#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include "alg_1.h"
#include "alg_2.h"
#include "alg_3.h"
#include "data_gen.h"

using namespace std;
using namespace chrono;

const int NUM_ALGORITHMS = 3;
const int NUM_TRIES = 100; 
const int MIN_VERTICES = 20;
const int MAX_VERTICES = 200;
const int STEP_VERTICES = 20;

struct MSTResult {
    int totalWeight;
    vector<pair<int, int>> edges;
};

int64_t measureAlgorithmTime(int algoNum, const vector<vector<int>>& graph, 
                             MSTResult& result) {
    auto start = high_resolution_clock::now();
    
    switch(algoNum) {
        case 1:
            result.totalWeight = kruskalMST(graph, result.edges);
            break;
        case 2:
            result.totalWeight = primMST(graph, result.edges);
            break;
        case 3:
            result.totalWeight = boruvkaMST(graph, result.edges);
            break;
    }
    
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> weightDist(1, 100);
    uniform_real_distribution<> probDist(0.0, 1.0);
    
    ofstream rezFile("rez.txt");
    if (!rezFile.is_open()) {
        cerr << "Error: Cannot open rez.txt for writing" << endl;
        return 1;
    }
    
    rezFile << "Vertices,Kruskal,Prim,Boruvka" << endl;
    
    cout << "Starting measurements..." << endl;
    cout << "Vertices\tKruskal\tPrim\tBoruvka" << endl;
    
    for (int vertices = MIN_VERTICES; vertices <= MAX_VERTICES; vertices += STEP_VERTICES) {
        int64_t totalTimes[NUM_ALGORITHMS] = {0};
        MSTResult result;
        
        for (int attempt = 0; attempt < NUM_TRIES; attempt++) {
            vector<vector<int>> graph = generateRandomGraph(vertices, weightDist, probDist, gen);
            
            for (int algo = 1; algo <= NUM_ALGORITHMS; algo++) {
                totalTimes[algo-1] += measureAlgorithmTime(algo, graph, result);
            }
        }
        
        rezFile << vertices;
        cout << vertices << "\t";
        
        for (int algo = 0; algo < NUM_ALGORITHMS; algo++) {
            double avgTimeNS = static_cast<double>(totalTimes[algo]) / NUM_TRIES;
            rezFile << "," << fixed << setprecision(0) << avgTimeNS;
            cout << fixed << setprecision(0) << avgTimeNS << "\t";
        }
        
        rezFile << "\n";
        cout << endl;
    }
    
    rezFile.close();
    cout << "Results saved to rez.txt" << endl;
    
    return 0;
}
