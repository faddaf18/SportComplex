#pragma once
#include <string>
#include "SportComplexException.h"

class ObjectNotFoundException : public SportComplexException {
private:
    std::string objectType;
    std::string key;

public:
    ObjectNotFoundException(const std::string& type, const std::string& objectKey, const std::string& where)
        : SportComplexException(type + " '" + objectKey + "' was not found in " + where + "."),
        objectType(type), key(objectKey) {}

    const std::string& get_object_type() const { return objectType; }
    const std::string& get_key() const { return key; }
};
