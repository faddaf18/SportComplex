#pragma once
#include <string>
#include "SportComplexException.h"

class BrokenRelationException : public SportComplexException {
public:
    explicit BrokenRelationException(const std::string& message)
        : SportComplexException("Relation between objects would be broken: " + message) {}
};

