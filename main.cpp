#include "Point.h"
#include "KDTree.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

void test0() {
    Point<3, double> p{1.0, 2.0, 3.0};
    Point<3, double> p1{0.0, 1.0, 1.0};
    Point<3, double> p2{-1.0, -5.0, -2.0};
    Point<3, double> p3{2.0, -1.0, 0.0};
    Point<3, double> p4{-2.0, 1.0, 1.0};
    Point<3, double> p5{4.0, 2.0, 1.0};
    Point<3, double> p6{-3.0, -1.0, -1.0};

    vector<Point<3, double>> pts {p, p1, p2, p3, p4, p5, p6};
    KDTree<3, Point<3, double>> index(pts);

    cout << index << endl;
}

void test1() {
    vector<Point<3, double>> pts;
    // Fixed the seed for reproducibility
    mt19937 twister(42);
    uniform_real_distribution<double> distribution(-100.0, 100.0);
    // Generate 100 points
    for (size_t i = 0; i < 100; ++i) {
        double x, y, z;
        for (size_t j = 0; j < 3; ++j) {
            x = distribution(twister);
            y = distribution(twister);
            z = distribution(twister);
        }

        pts.emplace_back( Point<3, double> {x, y, z});
    }

    KDTree<3, Point<3, double>> index(pts);
    cout << index << endl;
}

int main(void) {

    test1();
    return 0;
}