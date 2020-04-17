#pragma once

#include <string>
#include <stdexcept>

class AvlTreeEmptyException : public std::runtime_error {
public:
    AvlTreeEmptyException(const std::string& message);
};

class AvlTreeElementNotExistException : public std::runtime_error {
public:
    AvlTreeElementNotExistException(const std::string& message);
};

class AvlTreeRotationException : public std::runtime_error {
public:
    AvlTreeRotationException(const std::string& message);
};
