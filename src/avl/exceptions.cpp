#include <string>
#include <stdexcept>
#include <trees/avl/exceptions.hpp>

AvlTreeEmptyException::AvlTreeEmptyException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}

AvlTreeElementNotExistException::AvlTreeElementNotExistException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}

AvlTreeRotationException::AvlTreeRotationException(
    const std::string& message
    )
    : std::runtime_error(message)
{
}
