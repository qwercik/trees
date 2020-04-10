#pragma once

#include <string>
#include <stdexcept>

class BinarySearchTreeEmptyException : public std::runtime_error {
public:
    BinarySearchTreeEmptyException(const std::string& message);
};

class BinarySearchTreeElementNotExistException : public std::runtime_error {
public:
    BinarySearchTreeElementNotExistException(const std::string& message);
};

class BinarySearchTreeRotationException : public std::runtime_error {
public:
    BinarySearchTreeRotationException(const std::string& message);
};
