#pragma once
#include <stdexcept>
#include <string>

class SportComplexException : public std::runtime_error {
public:
    explicit SportComplexException(const std::string& message) : std::runtime_error(message) {}
};
