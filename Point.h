#ifndef POINT_H
#define POINT_H

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <type_traits>

template <std::size_t dim, typename T>
class Point {
public:
    using NumericType = T;
    Point(const std::initializer_list<T>& inList);
    Point(const Point& rhs);

    T calculateSqDistance(const Point<dim, T>& inValue) const;
    T calculateDistance(const Point<dim, T>& inValue) const;
    T calculateSqSplitDistance(const Point<dim, T>& inValue, size_t split) const;
    T calculateSplitDistance(const Point<dim, T>& inValue, size_t split) const;
    static constexpr std::size_t getDimension();
    const T& operator[] (const std::size_t d) const;
    bool operator== (const Point<dim, T>& rhs) const;
    bool operator!= (const Point<dim, T>& rhs) const;

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
    std::copy(inList.begin(), inList.end(), std::begin(val));
}

template <std::size_t dim, typename T>
Point<dim, T>::Point(const Point<dim, T>& rhs) {
    std::copy(std::begin(rhs.val), std::end(rhs.val), std::begin(val));
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateSqDistance(const Point<dim, T>& inPoint) const {
    T result = 0.0;

    for (size_t i = 0; i < dim; ++i) {
        result += (val[i] - inPoint.val[i]) * (val[i] - inPoint.val[i]);
    }

    return result;
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateDistance(const Point<dim, T>& inPoint) const {
    return std::sqrt(calculateSqDistance(inPoint));
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateSqSplitDistance(const Point<dim, T>& inPoint, size_t split) const {
    return (inPoint[split] - val[split]) * (inPoint[split] - val[split]);
}

template <std::size_t dim, typename T>
T Point<dim, T>::calculateSplitDistance(const Point<dim, T>& inPoint, size_t split) const {
    return std::sqrt(calculateSqSplitDistance(inPoint, split));
}

template <std::size_t dim, typename T>
constexpr std::size_t Point<dim, T>::getDimension() {
    return dim;
}

template <std::size_t dim, typename T>
const T& Point<dim, T>::operator[] (const std::size_t d) const {
    assert(d < dim && "Input dimension index exceeds data dimension!");
    return val[d];
}

template <std::size_t dim, typename T>
bool Point<dim, T>::operator== (const Point<dim, T>& rhs) const {
    bool equalFlag = true;

    if (std::is_floating_point<T>::value) {
        size_t ulp = 5;
        for (size_t i = 0; i < dim; ++i) {
            equalFlag &= (std::fabs(rhs[i] - val[i]) <= std::numeric_limits<T>::epsilon() * std::fabs(rhs[i] + val[i]) * ulp);
        }
    }
    else if (std::is_integral<T>::value) {
        for (size_t i = 0; i < dim; ++i) {
            equalFlag &= rhs[i] == val[i];
        }
    }
    else {
        throw std::invalid_argument("Invalid numerical type!");
    }

    return equalFlag;
}

template <std::size_t dim, typename T>
bool Point<dim, T>::operator!= (const Point<dim, T>& rhs) const {
    return !(*this == rhs);
}

#endif