#include "Coach.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/ConstraintViolationException.h"
#include <iostream>
#include <format>

Coach::Coach(std::string_view name, std::string_view s, int maxGroup)
    : lastName(name), sport(s), maxGroupSize(maxGroup) {
    if (name.empty()) {
        throw InvalidDataException("coach last name", "must not be empty");
    }
    if (s.empty()) {
        throw InvalidDataException("coach sport", "must not be empty");
    }
    if (maxGroup <= 0) {
        throw InvalidDataException("coach max group size", std::format("must be greater than 0 (got {})", maxGroup));
    }
}

void Coach::enroll(int people) {
    if (people <= 0) {
        throw InvalidDataException("number of athletes", std::format("must be greater than 0 (got {})", people));
    }
    if (currentGroupSize + people > maxGroupSize) {
        throw ConstraintViolationException("max group size",
            std::format("Cannot enroll {} athletes to coach {}. Exceeds max group limit ({}/{}).",
                people, lastName, currentGroupSize + people, maxGroupSize));
    }
    currentGroupSize += people;
}

void Coach::print_info(std::ostream& os) const {
    os << *this << "\n";
}