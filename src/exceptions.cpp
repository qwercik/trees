#include <string>
#include <stdexcept>
#include <trees/bst/exceptions.hpp>
#include <iostream>

BinarySearchTreeEmptyException::BinarySearchTreeEmptyException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}

BinarySearchTreeElementNotExistException::BinarySearchTreeElementNotExistException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}

BinarySearchTreeRotationException::BinarySearchTreeRotationException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}
