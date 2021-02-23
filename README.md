# kd_tree
A simple implementation of the k-d tree data structure with C++11. Currently supports point-like data with different dimensions and floating point types. 
The default metric is L2 (Euclidean distance).

## Prerequisite 
```
CMake
```
This code is tested on MacOS 11.2, Clang++ 12.0, and CMake 3.19.5. 
It should be fairly straight-forward to run on any Unix-based system. 

## Build and run
```
mkdir build
cd build
cmake ..
make
./main
```

# Usage
See `main.cpp` for the full test cases.
```
// Initialize 3d points and put them in a STL vector container
Point<3, double> pt0 {1.0, 2.0, 3.0};
....
std::vector<Point<3, double>> pts {pt0, pt1, pt2, pt3};
// Building the k-d tree
KDTree<Point<3, double>> index(pts);
// Search 5 nearest neighbors
Point<3, double> query {-1.0, 0.0, 0.0};
std:vector<Point<3, double>> results = index.searchKNearest(query, 5);
// result is sorted based on the distance to the query point in ascending order. 
```

## Future works
* Support metrics such as L1 and L-inf.
* Support on-manifold objects such as the rotation (SO2 and SO3) group.
* Current insert will lead to an unbalanced tree. Need to implement a re-balancing strategy. 
* Support tree pruning. 
* Use a bounded priority queue data structure to contain the result set of a KNN search. 
* Integral type is not tested. 
* Support tree building with repeated input values. 

## License
MIT
