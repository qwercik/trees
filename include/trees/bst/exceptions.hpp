#pragma once

#include <string>
#include <stdexcept>

class BinarySearchTreeEmptyException : public std::runtime_error {
public:
    BinarySearchTreeEmptyException(const std::string& message) : std::runtime_error(message) {
    }
};

class BinarySearchTreeElementNotExistException : public std::runtime_error {
public:
    BinarySearchTreeElementNotExistException(const std::string& message) : std::runtime_error(message) {
    }
};

class BinarySearchTreeRotationException : public std::runtime_error {
public:
    BinarySearchTreeRotationException(const std::string& message) : std::runtime_error(message) {
    }
};