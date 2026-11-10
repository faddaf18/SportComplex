#pragma once
#include <string>
#include "exceptions/SportComplexException.h"

class InvalidOperationException : public SportComplexException {
public:
    explicit InvalidOperationException(const std::string& message)
        : SportComplexException("Operation is not allowed: " + message) {}
};
