#pragma once
#include <string>
#include "exceptions/SportComplexException.h"

class InvalidDataException : public SportComplexException {
private:
    std::string field;

public:
    InvalidDataException(const std::string& fieldName, const std::string& reason)
        : SportComplexException("Invalid data in '" + fieldName + "': " + reason), field(fieldName) {}

    const std::string& get_field() const { return field; }
};
