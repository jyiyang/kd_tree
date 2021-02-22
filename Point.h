#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <type_traits>

template <std::size_t dim, typename T>
class Point {
public:
    using NumericType = T;
    Point(const std::initializer_list<T>& inList);

    T calculateDistance(const Point<dim, T>& inValue) const;
    T calculateSplitDistance(const Point<dim, T>& inValue, size_t split) const;
    static constexpr std::size_t getDimension();
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
T Point<dim, T>::calculateDistance(const Point<dim, T>& inPoint) const {
    T result = 0.0;

    for (size_t i = 0; i < dim; ++i) {
        result += (this->val[i] - inPoint.val[i]) * (this->val[i] - inPoint.val[i]);
    }

    return std::sqrt(result);
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateSplitDistance(const Point<dim, T>& inPoint, size_t split) const {
    return std::fabs(inPoint[split] - this->val[split]);
}

template <std::size_t dim, typename T>
constexpr std::size_t Point<dim, T>::getDimension() {
    return dim;
}

template <std::size_t dim, typename T>
const T& Point<dim, T>::operator[] (const std::size_t d) const {
    return this->val[d];
}

#endif