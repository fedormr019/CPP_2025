#include <iostream>

using namespace std;

struct QuadraticEquation {
    int a, b, c;
    pair<double, double> solve() {
        double d = b * b - 4 * a * c;
        if (d < 0) return { std::nan(""), std::nan("") };
        double x1 = (-b + sqrt(d)) / (2 * a);
        double x2 = (-b - sqrt(d)) / (2 * a);
        return { x1, x2 };
    }
};

struct Submission {
    QuadraticEquation eq;
    pair<double, double> answer;
    string studentName;
};