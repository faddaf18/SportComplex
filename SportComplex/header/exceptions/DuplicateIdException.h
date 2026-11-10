#pragma once
#include <string>
#include "exceptions/SportComplexException.h"

class DuplicateIdException : public SportComplexException {
private:
    std::string objectType;
    std::string id;

public:
    DuplicateIdException(const std::string& type, const std::string& objectId, const std::string& where)
        : SportComplexException(type + " with identifier '" + objectId + "' already exists in " + where + "."),
        objectType(type), id(objectId) {}

    const std::string& get_object_type() const { return objectType; }
    const std::string& get_id() const { return id; }
};
