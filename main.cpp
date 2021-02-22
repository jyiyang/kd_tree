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

    vector<Point<3, double>> pts {p1, p2, p3, p4, p5, p6};
    KDTree<Point<3, double>> index(pts);

    auto res = index.searchKNearest(p, 3);

    for (auto& r: res) {
        cout << r << endl;
    }
}

void test1() {
    constexpr size_t dim = 2;
    size_t numPts = 10000;
    vector<Point<dim, double>> pts;
    // Fixed the seed for reproducibility
    mt19937 twister(42);
    uniform_real_distribution<double> distribution(-100.0, 100.0);
    // Generate 100 points
    for (size_t i = 0; i < numPts; ++i) {
        double x, y;
        for (size_t j = 0; j < 2; ++j) {
            x = distribution(twister);
            y = distribution(twister);
        }

        pts.emplace_back( Point<dim, double> {x, y});
    }

    Point<dim, double> query{0.0, 0.0};

    KDTree<Point<dim, double>> index(pts);
    auto res = index.searchKNearest(query, 3);

    for (auto& r: res) {
        cout << r << endl;
    }

    // Brute force search to get ground truth
    sort(pts.begin(), pts.end(), [query](const Point<dim, double>& a, const Point<dim, double>& b) {
        auto dist1 = a.calculateDistance(query);
        auto dist2 = b.calculateDistance(query);

        return dist1 < dist2;
    });

    for (size_t i = 0; i < 3; ++i) {
        cout << pts[i] << endl;
    }
}

void test2() {
    constexpr size_t dim = 3;
    size_t numPts = 10000;

    vector<Point<dim, double>> pts;

    // Fixed the seed for reproducibility
    mt19937 twister(42);
    uniform_real_distribution<double> distribution(-100.0, 100.0);
    for (size_t i = 0; i < numPts; ++i) {
        double x, y, z;
        for (size_t j = 0; j < dim; ++j) {
            x = distribution(twister);
            y = distribution(twister);
            z = distribution(twister);
        }

        pts.emplace_back( Point<dim, double> {x, y, z});
    }

    Point<3, double> query{0.0, 0.0, 0.0};

    KDTree<Point<dim, double>> index(pts);
    auto res = index.searchKNearest(query, 3);

    for (auto& r: res) {
        cout << r << endl;
    }

    // Brute force search to get ground truth
    sort(pts.begin(), pts.end(), [query](const Point<3, double>& a, const Point<dim, double>& b) {
        auto dist1 = a.calculateDistance(query);
        auto dist2 = b.calculateDistance(query);

        return dist1 < dist2;
    });

    for (size_t i = 0; i < 3; ++i) {
        cout << pts[i] << endl;
    }
}

int main(void) {
    // test0();
    // test1();
    test2();
    return 0;
}