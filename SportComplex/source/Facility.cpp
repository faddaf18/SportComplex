#include "Facility.h"

Facility::Facility(std::string_view n, std::string_view t, int cap)
    : name(n), type(t) {
    set_capacity(cap);
}

void Facility::add_sport(std::string_view sportName) {
    if (sportCount < SPORTSCOUNT) {
        sports.at(sportCount) = std::string(sportName);
        sportCount++;
    }
    else {
        std::cout << "Sports limit reached for this facility (max 3).\n";
    }
}

void Facility::clear_and_reset_sports() {
    sportCount = 0;
    std::cout << "How many sports will be available (1 to 3)? ";
    int sCount = 0;
    std::cin >> sCount;
    std::cin.ignore();
    for (int i = 0; i < sCount && i < SPORTSCOUNT; i++) {
        std::string sport;
        std::cout << "Enter sport #" << (i + 1) << ": ";
        std::getline(std::cin, sport);
        add_sport(sport);
    }
}

void Facility::set_capacity(int newCap) {
    if (newCap > 0) {
        capacity = newCap;
    }
    else {
        std::cout << "Error: Capacity must be greater than 0! Default value set: 10.\n";
        capacity = 10;
    }
}

void Facility::set_name(std::string_view newName) {
    name = newName;
}

void Facility::print_short_info() const {
    std::cout << "Facility: " << name << " (" << type << ") | Capacity: " << capacity << " people\n";
}

bool Facility::check_training(std::string_view sport, int people) const {
    if (people > capacity) {
        std::cout << "-> Denied: Group of " << people << " people exceeds facility capacity (" << capacity << ").\n";
        return false;
    }
    bool hasSport = false;
    for (std::size_t i = 0; i < sportCount; i++) {
        if (sports.at(i) == sport) {
            hasSport = true;
            break;
        }
    }
    if (!hasSport) {
        std::cout << "-> Denied: Facility '" << name << "' does not support sport: " << sport << ".\n";
        return false;
    }
    return true;
}

bool Facility::operator==(const Facility& other) const {
    return name == other.name;
}