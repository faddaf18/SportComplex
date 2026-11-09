#include "Coach.h"
#include "InvalidDataException.h"
#include "ConstraintViolationException.h"
#include <iostream>

Coach::Coach(std::string_view name, std::string_view s, int maxGroup)
    : lastName(name), sport(s), maxGroupSize(maxGroup) {
    if (name.empty()) {
        throw InvalidDataException("coach last name", "must not be empty");
    }
    if (s.empty()) {
        throw InvalidDataException("coach sport", "must not be empty");
    }
    if (maxGroup <= 0) {
        throw InvalidDataException("coach max group size", "must be greater than 0 (got " + std::to_string(maxGroup) + ")");
    }
}

void Coach::enroll(int people) {
    if (people <= 0) {
        throw InvalidDataException("number of athletes", "must be greater than 0 (got " + std::to_string(people) + ")");
    }
    if (currentGroupSize + people > maxGroupSize) {
        throw ConstraintViolationException("max group size",
            "Cannot enroll " + std::to_string(people) + " athletes to coach " + lastName +
            ". Exceeds max group limit (" + std::to_string(currentGroupSize + people) + "/" +
            std::to_string(maxGroupSize) + ").");
    }
    currentGroupSize += people;
}

void Coach::print_info(std::ostream& os) const {
    os << *this << "\n";
}
