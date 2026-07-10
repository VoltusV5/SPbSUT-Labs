#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "alg_1.h"
#include "alg_2.h"
#include "alg_3.h"

using namespace std;
using namespace chrono;

int main() {
    srand(time(0));
    ofstream res("graph_bench.csv");
    res << "Vertices,Dijkstra,BellmanFord,FloydWarshall\n";

    // Идём до 300 вершин (Флойд на 300 уже заметно тормозит)
    for (int v = 10; v <= 300; v += 20) {
        vector<vector<pair<int, int>>> adj(v);
        vector<tuple<int, int, int>> edges;
        vector<vector<int>> matrix(v, vector<int>(v, 1e9));
        for(int i=0; i<v; ++i) matrix[i][i] = 0;

        // Генерация случайных связей
        for (int i = 0; i < v; i++) {
            for (int j = 0; j < 10; j++) { // Плотность графа
                int to = rand() % v;
                int weight = rand() % 100;
                if (i == to) continue;
                adj[i].push_back({to, weight});
                edges.push_back(make_tuple(i, to, weight));
                matrix[i][to] = weight;
            }
        }

        cout << "V = " << v << "... " << flush;

        auto s1 = high_resolution_clock::now();
        solveDijkstra(v, adj);
        double t1 = duration<double>(high_resolution_clock::now() - s1).count();

        auto s2 = high_resolution_clock::now();
        solveBellmanFord(v, edges);
        double t2 = duration<double>(high_resolution_clock::now() - s2).count();

        auto s3 = high_resolution_clock::now();
        solveFloydWarshall(v, matrix);
        double t3 = duration<double>(high_resolution_clock::now() - s3).count();

        res << v << "," << t1 << "," << t2 << "," << t3 << "\n";
        cout << "Done." << endl;
    }
    return 0;
}