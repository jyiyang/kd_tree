#include "Point.h"
#include "KDTree.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

// helper to generate expected value
template <size_t dim, typename T> 
vector<Point<dim, T>> bruteForceSearch(const Point<dim, T>& query, size_t k, vector<Point<dim, T>>& input) {
    vector<Point<dim, T>> result; 

    sort(input.begin(), input.end(), [query](const Point<dim, T>& a, const Point<dim, T>& b) {
        auto dist1 = a.calculateSqDistance(query);
        auto dist2 = b.calculateSqDistance(query);

        return dist1 < dist2;
    });

    for (size_t i = 0; i < k; ++i) {
        result.push_back(input[i]);
    }

    return result;
}

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
    cout << "======================" << endl;
    constexpr size_t dim = 2;
    size_t numPts = 10000;
    size_t kNum = 3;
    cout << "Test with " << numPts << " points with dimension " << dim << endl;

    vector<Point<dim, double>> pts;
    // Fixed the seed for reproducibility
    mt19937 twister(42);
    uniform_real_distribution<double> distribution(-100.0, 100.0);
    for (size_t i = 0; i < numPts; ++i) {
        double x, y;
        for (size_t j = 0; j < dim; ++j) {
            x = distribution(twister);
            y = distribution(twister);
        }

        pts.emplace_back( Point<dim, double> {x, y});
    }

    Point<dim, double> query{0.0, 0.0};

    // Build index
    KDTree<Point<dim, double>> index(pts);

    // Perform knn search using kd tree
    auto kdStartTime = chrono::system_clock::now();
    auto actual = index.searchKNearest(query, kNum);
    auto kdEndTime = chrono::system_clock::now();
    auto kdElapsed = chrono::duration_cast<chrono::microseconds>(kdEndTime - kdStartTime);

    // Perform brute force search to get expected
    auto bfStartTime = chrono::system_clock::now();
    auto expected = bruteForceSearch(query, kNum, pts);
    auto bfEndTime = chrono::system_clock::now();
    auto bfElapsed = chrono::duration_cast<chrono::microseconds>(bfEndTime - bfStartTime);

    auto swidth = 20, width = 40;
    cout << left
         << setw(width) << "Expected closest point: "
         << setw(width) << "Actual closest point from kd-tree: "
         << setw(width) << "Same point?" << endl;
    for (size_t i = 0; i < kNum; ++i) {
        string diff = std::fabs(expected[i].calculateDistance(actual[i])) < 1e-7 ? "yes" : "no";
        cout << left
             << expected[i]
             << setw(swidth) << "  " << actual[i]
             << setw(swidth) << "  " << diff << endl;
    }

    cout << "======================" << endl;
    cout << "Time to search " << kNum << " closest point(s) from " << numPts << " points with dimension " << dim << endl;
    cout << "KD tree search time: " << kdElapsed.count() << " us." << endl;
    cout << "Brute force search time: " << bfElapsed.count() << " us." << endl;
    cout << "======================" << endl;
}

void test2() {
    cout << "======================" << endl;
    constexpr size_t dim = 3;
    size_t numPts = 1000000;
    size_t kNum = 5;
    cout << "Test with " << numPts << " points with dimension " << dim << endl;

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

    Point<dim, double> query{0.0, 0.0, 0.0};

    // Building the kd-tree index
    KDTree<Point<dim, double>> index(pts);

    // Perform knn search using kd tree
    auto kdStartTime = chrono::system_clock::now();
    auto actual = index.searchKNearest(query, kNum);
    auto kdEndTime = chrono::system_clock::now();
    auto kdElapsed = chrono::duration_cast<chrono::microseconds>(kdEndTime - kdStartTime);

    // Perform brute force search to get expected
    auto bfStartTime = chrono::system_clock::now();
    auto expected = bruteForceSearch(query, kNum, pts);
    auto bfEndTime = chrono::system_clock::now();
    auto bfElapsed = chrono::duration_cast<chrono::microseconds>(bfEndTime - bfStartTime);

    auto swidth = 20, width = 40;
    cout << left
         << setw(width) << "Expected closest point: "
         << setw(width) << "Actual closest point from kd-tree: "
         << setw(width) << "Same point?" << endl;
    for (size_t i = 0; i < kNum; ++i) {
        string diff = std::fabs(expected[i].calculateDistance(actual[i])) < 1e-7 ? "yes" : "no";
        cout << left
             << expected[i]
             << setw(swidth) << "  " << actual[i]
             << setw(swidth) << "  " << diff << endl;
    }

    cout << "======================" << endl;
    cout << "Time to search " << kNum << " closest point(s) from " << numPts << " points with dimension " << dim << endl;
    cout << "KD tree search time: " << kdElapsed.count() << " us." << endl;
    cout << "Brute force search time: " << bfElapsed.count() << " us." << endl;
    cout << "======================" << endl;
}

int main(void) {
    // test0();
    test1();
    test2();
    return 0;
}