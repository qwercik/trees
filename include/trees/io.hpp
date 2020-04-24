#pragma once

#include <iostream>
#include <vector>

#define DBG(x) std::cerr << "[DBG] " << #x << " = " << x << '\n';

template <typename T, typename Container>
std::ostream& operator<<(std::ostream& stream, const Container& vector) {
    const std::string SEPARATOR = ", ";

    if (vector.size() != 0) {
        for (int index = 0; index < vector.size() - 1; ++index) {
            stream << vector[index] << SEPARATOR;
        }

        stream << vector[vector.size() - 1];
    }

    return stream;
}


template <typename T>
std::vector<T> readList(std::istream& stream = std::cin) {
    std::vector<T> values;
    T value;
    while (stream >> value) {
        values.push_back(value);
    }

    return values;
}