#pragma once
#include <string>
#include "SportComplexException.h"

class ConstraintViolationException : public SportComplexException {
private:
    std::string constraint;

public:
    ConstraintViolationException(const std::string& constraintName, const std::string& message)
        : SportComplexException(message), constraint(constraintName) {}

    const std::string& get_constraint() const { return constraint; }
};

