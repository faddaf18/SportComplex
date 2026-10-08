#include "Coach.h"
#include <iostream>

Coach::Coach(std::string_view name, std::string_view s, int maxGroup)
    : lastName(name), sport(s), maxGroupSize(maxGroup > 0 ? maxGroup : 15) {}

bool Coach::enroll(int people) {
    if (people <= 0) return false;
    if (currentGroupSize + people <= maxGroupSize) {
        currentGroupSize += people;
        return true;
    }
    return false;
}

void Coach::print_info() const {
    std::cout << "Coach: " << lastName
        << " | Sport: " << sport
        << " | Group: " << currentGroupSize << "/" << maxGroupSize << "\n";
}