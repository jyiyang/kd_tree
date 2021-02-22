#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <type_traits>

template <std::size_t dim, typename T>
class Point {
public:
    Point(const std::initializer_list<T>& inList);

    T calculateDistance(const Point<dim, T>& inValue);
    std::size_t getDimension();
    const T& operator[](const std::size_t d) const;

    friend std::ostream& operator<< (std::ostream& out, const Point<dim, T>& point) {
        out << "[";
        for (size_t i = 0; i < dim - 1; ++i) {
            out << point.val[i] << ", ";
        }

        out << point.val[dim - 1] << "]";

        return out;
    }

private:
    T val[dim];
};

template <std::size_t dim, typename T>
Point<dim, T>::Point(const std::initializer_list<T>& inList) {
    assert("Inconsistent dimension!" && inList.size() == dim);
    std::copy(inList.begin(), inList.end(), this->val);
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateDistance(const Point<dim, T>& inPoint) {
    T result = T(0.0);

    for (size_t i = 0; i < dim; ++i) {
        result += (this->val[i] - inPoint.val[i]) * (this->val[i] - inPoint.val[i]);
    }

    return result;
}

template <std::size_t dim, typename T>
std::size_t Point<dim, T>::getDimension() {
    return dim;
}

template <std::size_t dim, typename T>
const T& Point<dim, T>::operator[] (const std::size_t d) const {
    return this->val[d];
}

#endif