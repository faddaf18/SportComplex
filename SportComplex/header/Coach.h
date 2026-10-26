#pragma once
#include <iostream>
#include <string>
#include <string_view>

class Coach {
private:
    std::string lastName = "Unknown";
    std::string sport = "Unknown";
    int maxGroupSize = 15;
    int currentGroupSize = 0;

public:
    Coach() = default;
    Coach(std::string_view name, std::string_view s, int maxGroup);

    std::string_view get_last_name() const { return lastName; }
    std::string_view get_sport() const { return sport; }
    int get_max_group_size() const { return maxGroupSize; }
    int get_current_group_size() const { return currentGroupSize; }

    bool enroll(int people);
    void print_info(std::ostream& os = std::cout) const;

    friend std::ostream& operator<<(std::ostream& os, const Coach& c) {
        os << "Coach: " << c.lastName
            << " | Sport: " << c.sport
            << " | Group: " << c.currentGroupSize << "/" << c.maxGroupSize;
        return os;
    }
};