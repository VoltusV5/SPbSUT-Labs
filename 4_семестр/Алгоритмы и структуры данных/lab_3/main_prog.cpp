#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

// =============================================
// Функция правой части дифференциального уравнения
// y' = a*y + b*sin(x) + c
// =============================================
double f(double x, double y, const vector<double>& params) {
    double a = params[0];
    double b = params[1];
    double c = params[2];
    return a * y + b * sin(x) + c;
}

// =============================================
// Метод Эйлера
// =============================================
void solveEuler(double x0, double y0, double h, int steps, 
                const vector<double>& params) {
    double x = x0;
    double y = y0;
    
    for (int i = 0; i < steps; i++) {
        y = y + h * f(x, y, params);
        x = x + h;
    }
}

// =============================================
// Метод Рунге-Кутты 4-го порядка
// =============================================
void solveRungeKutta(double x0, double y0, double h, int steps, 
                     const vector<double>& params) {
    double x = x0;
    double y = y0;
    
    for (int i = 0; i < steps; i++) {
        double k1 = h * f(x, y, params);
        double k2 = h * f(x + h/2, y + k1/2, params);
        double k3 = h * f(x + h/2, y + k2/2, params);
        double k4 = h * f(x + h, y + k3, params);
        
        y = y + (k1 + 2*k2 + 2*k3 + k4) / 6;
        x = x + h;
    }
}

// =============================================
// Метод Адамса-Башфорта 3-го порядка
// =============================================
void solveAdamsBashforth(double x0, double y0, double h, int steps, 
                         const vector<double>& params) {
    if (steps < 3) {
        // Если шагов мало — просто используем Рунге-Кутту
        solveRungeKutta(x0, y0, h, steps, params);
        return;
    }

    double x = x0;
    double y = y0;
    
    vector<double> y_vals(3);
    vector<double> f_vals(3);
    
    y_vals[0] = y0;
    f_vals[0] = f(x, y_vals[0], params);
    
    // Получаем y1 с помощью Рунге-Кутты
    {
        double k1 = h * f(x, y_vals[0], params);
        double k2 = h * f(x + h/2, y_vals[0] + k1/2, params);
        double k3 = h * f(x + h/2, y_vals[0] + k2/2, params);
        double k4 = h * f(x + h, y_vals[0] + k3, params);
        y_vals[1] = y_vals[0] + (k1 + 2*k2 + 2*k3 + k4) / 6;
        f_vals[1] = f(x + h, y_vals[1], params);
    }
    
    // Получаем y2 с помощью Рунге-Кутты
    x = x0 + h;
    {
        double k1 = h * f(x, y_vals[1], params);
        double k2 = h * f(x + h/2, y_vals[1] + k1/2, params);
        double k3 = h * f(x + h/2, y_vals[1] + k2/2, params);
        double k4 = h * f(x + h, y_vals[1] + k3, params);
        y_vals[2] = y_vals[1] + (k1 + 2*k2 + 2*k3 + k4) / 6;
        f_vals[2] = f(x + h, y_vals[2], params);
    }
    
    x = x0 + 2 * h;
    y = y_vals[2];
    
    // Основной цикл Адамса-Башфорта 3-го порядка
    for (int i = 3; i < steps; i++) {
        double y_next = y + h * (23 * f_vals[2] - 16 * f_vals[1] + 5 * f_vals[0]) / 12;
        
        // Сдвиг значений
        f_vals[0] = f_vals[1];
        f_vals[1] = f_vals[2];
        f_vals[2] = f(x + h, y_next, params);
        
        y = y_next;
        x = x + h;
    }
}

// =============================================
// Генерация тестовых данных
// =============================================
void generateData(const char* filename) {
    srand(static_cast<unsigned>(time(nullptr)));
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Cannot open file for writing!" << endl;
        return;
    }
    
    file << scientific << setprecision(8);
    
    double x0 = (rand() % 200 - 100) / 10.0;   // от -10 до 10
    double y0 = (rand() % 200 - 100) / 10.0;
    
    file << x0 << " " << y0 << endl;
    
    // Параметры a, b, c
    for (int i = 0; i < 3; i++) {
        double param = (rand() % 200 - 100) / 10.0;
        file << param << " ";
    }
    
    file.close();
    
    cout << "  Data generated successfully in file: " << filename << endl;
    cout << "  Initial point: (" << fixed << setprecision(1) 
         << x0 << ", " << y0 << ")" << endl;
}

// =============================================
// Измерение времени выполнения метода
// =============================================
double measureTime(int methodNum, double x0, double y0, double h, int steps, 
                   const vector<double>& params) {
    auto start = high_resolution_clock::now();
    
    switch(methodNum) {
        case 1: solveEuler(x0, y0, h, steps, params); break;
        case 2: solveRungeKutta(x0, y0, h, steps, params); break;
        case 3: solveAdamsBashforth(x0, y0, h, steps, params); break;
    }
    
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    return diff.count();
}

// =============================================
// Главная функция
// =============================================
int main() {
    const int NUM_METHODS = 3;
    const int NUM_TRIES = 10000;
    const int NUM_POINTS = 8;
    
    cout << "==========================================================" << endl;
    cout << "COMPARISON OF DIFFERENTIAL EQUATION SOLVING METHODS" << endl;
    cout << "==========================================================" << endl;
    cout << "Methods being compared:" << endl;
    cout << "  1. Euler's Method" << endl;
    cout << "  2. Runge-Kutta 4th Order Method" << endl;
    cout << "  3. Adams-Bashforth 3rd Order Method" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "Measurement parameters:" << endl;
    cout << "  Number of measurements per point: " << NUM_TRIES << endl;
    cout << "  Number of step size points: " << NUM_POINTS << endl;
    cout << "  Initial step size (h): 0.1" << endl;
    cout << "  Final step size (h): 0.1/2^7 ≈ 0.00078125" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    // Генерация данных
    cout << "Generating initial data..." << endl;
    generateData("data.txt");
    cout << "----------------------------------------------------------" << endl;
    
    // Чтение параметров
    ifstream dataFile("data.txt");
    double x0, y0;
    vector<double> params;
    
    dataFile >> x0 >> y0;
    double param;
    while (dataFile >> param) {
        params.push_back(param);
    }
    dataFile.close();
    
    cout << "Initial conditions loaded from data.txt:" << endl;
    cout << "  x0 = " << x0 << ", y0 = " << y0 << endl;
    cout << "  Equation parameters (a, b, c): ";
    for (size_t i = 0; i < params.size(); i++) {
        cout << params[i] << (i < params.size() - 1 ? ", " : "");
    }
    cout << endl;
    cout << "  Differential equation: y' = a*y + b*sin(x) + c" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    // Файл для результатов
    ofstream rezFile("rez.txt");
    rezFile << "h\tEuler\tRungeKutta\tAdamsBashforth" << endl;
    
    cout << "Warming up..." << endl;
    for (int method = 1; method <= NUM_METHODS; method++) {
        measureTime(method, x0, y0, 0.1, 10, params);
    }
    cout << "Warm-up complete." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    cout << "Starting measurements..." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    double h = 0.1;
    for (int point = 0; point < NUM_POINTS; point++) {
        int steps = static_cast<int>(1.0 / h);
        
        cout << "Point " << (point + 1) << "/" << NUM_POINTS 
             << " | h = " << scientific << setprecision(8) << h 
             << " | steps = " << steps << fixed << endl;
        
        rezFile << scientific << setprecision(8) << h << "\t";
        
        for (int method = 1; method <= NUM_METHODS; method++) {
            double totalTime = 0.0;
            
            for (int attempt = 0; attempt < NUM_TRIES; attempt++) {
                totalTime += measureTime(method, x0, y0, h, steps, params);
            }
            
            double avgTime = totalTime / NUM_TRIES;
            rezFile << scientific << setprecision(6) << avgTime;
            
            if (method < NUM_METHODS) rezFile << "\t";
            
            cout << "    Method " << method << " complete. Avg time: " 
                 << scientific << setprecision(6) << avgTime << " s" << fixed << endl;
        }
        rezFile << "\n";
        
        h /= 2.0;
        
        cout << "  Completed for h = " << scientific << setprecision(8) << h * 2 << fixed << endl;
        cout << "  Remaining points: " << (NUM_POINTS - point - 1) << endl;
        cout << "----------------------------------------------------------" << endl;
    }
    
    rezFile.close();
    
    cout << "==========================================================" << endl;
    cout << "MEASUREMENTS COMPLETE" << endl;
    cout << "==========================================================" << endl;
    cout << "Results saved to file: rez.txt" << endl << endl;
    
    cout << "File format (tab-separated):" << endl;
    cout << "  Column 1: Step size (h)" << endl;
    cout << "  Column 2: Euler method average time (seconds)" << endl;
    cout << "  Column 3: Runge-Kutta method average time (seconds)" << endl;
    cout << "  Column 4: Adams-Bashforth method average time (seconds)" << endl << endl;
    
    cout << "To plot graphs in Excel:" << endl;
    cout << "  1. Open rez.txt in Excel" << endl;
    cout << "  2. Select all data" << endl;
    cout << "  3. Insert → Scatter plot with smooth lines" << endl;
    cout << "  4. X-axis: step size (h)" << endl;
    cout << "  5. Y-axis: average time (seconds)" << endl;
    cout << "==========================================================" << endl;
    
    return 0;
}