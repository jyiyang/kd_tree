#ifndef VALUE_H
#define VALUE_H

#include <cmath>

template <size_t Dim, typename T>
class Value {
public:
    virtual T calculateDistance(const Value<Dim, T>& inValue) = 0;
};

#endif
