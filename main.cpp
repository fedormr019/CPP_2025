#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "structs.hpp"

using namespace std;

class Student {
public:
    string name;
    virtual pair<double, double> solve(QuadraticEquation eq) = 0;
};

class GoodStudent : public Student {
public:
    GoodStudent(string n) { name = n; } 
    pair<double, double> solve(QuadraticEquation eq) override {
        return eq.solve();
    }
};

class AverageStudent : public Student {
public:
    AverageStudent(string n) { name = n; } 
    pair<double, double> solve(QuadraticEquation eq) override {
        if (rand() % 2) return eq.solve();
        return { rand() % 10, rand() % 10 };
    }
};

class BadStudent : public Student {
public:
    BadStudent(string n) { name = n; }  
    pair<double, double> solve(QuadraticEquation) override {
        return { 0, 0 };
    }
};

class Professor {
private:
    queue<Submission> submissions;
    map<string, int> scores;
public:
    void receiveSubmission(Submission sub) {
        submissions.push(sub);
    }
    void evaluate() {
        while (!submissions.empty()) {
            Submission sub = submissions.front();
            submissions.pop();
            pair<double, double> correct = sub.eq.solve();
            if (sub.answer == correct) {
                scores[sub.studentName]++;
            }
        }
    }
    void printResultsToFile(const string& filename) {
        ofstream outFile(filename);
        for (auto& entry : scores) {
            outFile << entry.first << " solved " << entry.second << " problems correctly." << endl;
        }
        outFile.close();
    }
};

int main() {
    srand(time(0));
    vector<Student*> students = { new GoodStudent{"Victor"}, new AverageStudent{"Oleg"}, new AverageStudent{"Nikola"}, new BadStudent{"Andrew"} };
    Professor prof;
    ifstream file("equations.txt");
    int a, b, c;
    while (file >> a >> b >> c) {
        QuadraticEquation eq{ a, b, c };
        for (auto student : students) {
            prof.receiveSubmission({ eq, student->solve(eq), student->name });
        }
    }
    prof.evaluate();
    prof.printResultsToFile("results.txt");
    for (auto student : students) delete student;
    return 0;
}