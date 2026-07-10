#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Nanoseconds = std::chrono::nanoseconds;


void bubbleSort(std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
            }
        }
    }
}


void selectionSort(std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(a[i], a[minIndex]);
        }
    }
}


void insertionSort(std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    for (int i = 1; i < n; ++i) {
        const int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

std::vector<int> randomArray(int n) {
    std::vector<int> a(n);
    for (int& x : a) {
        x = std::rand() % 20001 - 10000;
    }
    return a;
}

double benchmarkNs(const std::vector<int>& input,
                  void (*sortFunc)(std::vector<int>&),
                  int trials) {
    std::vector<int> check = input;
    sortFunc(check);
    if (!std::is_sorted(check.begin(), check.end())) {
        throw std::runtime_error("Sort validation failed.");
    }

    long long totalNs = 0;
    for (int t = 0; t < trials; ++t) {
        std::vector<int> data = input;
        const auto t1 = Clock::now();
        sortFunc(data);
        const auto t2 = Clock::now();
        totalNs += std::chrono::duration_cast<Nanoseconds>(t2 - t1).count();
    }
    return static_cast<double>(totalNs) / static_cast<double>(trials);
}

int main() {
    std::srand(42);

    std::vector<int> sizes;
    for (int n = 500; n <= 10000; n += 500) {
        sizes.push_back(n);
    }

    const int trials = 10000;

    std::ofstream csvRandom("benchmark_random.csv", std::ios::out);
    std::ofstream csvAll("benchmark_results.csv", std::ios::out);
    csvRandom << "n,bubble_s,selection_s,insertion_s\n";
    csvAll << "dataset,n,bubble_s,selection_s,insertion_s\n";

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Algorithms: Bubble sort, Selection sort, Insertion sort\n";
    std::cout << "Sizes: 500..10000, step 500 (20 points)\n";
    std::cout << "Trials per size: " << trials << "\n\n";
    std::cout << "n\tbubble(s)\tselection(s)\tinsertion(s)\n";

    for (int n : sizes) {
        const std::vector<int> input = randomArray(n);

        const double bubbleNs = benchmarkNs(input, bubbleSort, trials);
        const double selectionNs = benchmarkNs(input, selectionSort, trials);
        const double insertionNs = benchmarkNs(input, insertionSort, trials);

        const double bubbleS = bubbleNs / 1e9;
        const double selectionS = selectionNs / 1e9;
        const double insertionS = insertionNs / 1e9;

        std::cout << n << "\t" << bubbleS << "\t" << selectionS << "\t" << insertionS << "\n";
        csvRandom << n << "," << bubbleS << "," << selectionS << "," << insertionS << "\n";
        csvAll << "random," << n << "," << bubbleS << "," << selectionS << "," << insertionS << "\n";
    }

    csvRandom.close();
    csvAll.close();

    std::cout << "\nSaved: benchmark_random.csv, benchmark_results.csv\n";
    return 0;
}
