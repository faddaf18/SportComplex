#include "Facility.h"

Facility::Facility(std::string_view n, int cap) : name(n) {
    set_capacity(cap);
}

void Facility::add_sport(std::string_view sportName) {
    if (sportCount < SPORTS_COUNT) {
        sports.at(sportCount) = std::string(sportName);
        sportCount++;
    }
    else {
        std::cout << "Sports limit reached for this facility (max 3).\n";
    }
}

bool Facility::has_sport(std::string_view sportName) const {
    for (std::size_t i = 0; i < sportCount; ++i) {
        if (sports.at(i) == sportName) {
            return true;
        }
    }
    return false;
}

void Facility::clear_and_reset_sports() {
    sportCount = 0;
    std::cout << "How many sports will be available (1 to 3)? ";
    int sCount = 0;
    std::cin >> sCount;
    std::cin.ignore();
    for (int i = 0; i < sCount && i < SPORTS_COUNT; i++) {
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
    std::cout << "Facility: " << name << " (" << get_type_name() << ") | Capacity: " << capacity << " people\n";
}

bool Facility::add_coach(std::string_view lastName, std::string_view sport, int maxGroup) {
    if (!has_sport(sport)) {
        std::cout << "-> ERROR: Facility '" << name << "' does not support sport: " << sport << "!\n";
        return false;
    }
    if (maxGroup > capacity) {
        std::cout << "-> ERROR: Coach group size (" << maxGroup << ") exceeds facility capacity (" << capacity << ")!\n";
        return false;
    }
    coaches.emplace_back(lastName, sport, maxGroup);
    std::cout << "-> SUCCESS: Coach " << lastName << " (" << sport << ") added to " << name << ".\n";
    return true;
}

bool Facility::enroll_to_coach(std::string_view coachLastName, int people) {
    for (auto& coach : coaches) {
        if (coach.get_last_name() == coachLastName) {
            if (coach.enroll(people)) {
                std::cout << "-> SUCCESS: Enrolled " << people << " athletes to coach "
                    << coachLastName << ". Group status: "
                    << coach.get_current_group_size() << "/" << coach.get_max_group_size() << ".\n";
                return true;
            }
            else {
                std::cout << "-> DENIED: Cannot enroll " << people << " athletes to coach "
                    << coachLastName << ". Exceeds max group limit ("
                    << coach.get_current_group_size() + people << "/" << coach.get_max_group_size() << ").\n";
                return false;
            }
        }
    }
    std::cout << "-> ERROR: Coach '" << coachLastName << "' not found in facility '" << name << "'!\n";
    return false;
}

void Facility::show_coaches() const {
    if (coaches.empty()) {
        std::cout << "No coaches assigned to this facility.\n";
        return;
    }
    for (const auto& coach : coaches) {
        coach.print_info();
    }
}

void Facility::print_full_info(std::ostream& os) const {
    os << "Facility: " << name
        << "\nType: " << get_type_name()
        << "\nCapacity: " << capacity << " people"
        << "\nMax Daily Capacity: " << calculate_max_daily_capacity() << " people"
        << "\nSports: ";
    if (sportCount == 0) {
        os << "None";
    }
    else {
        for (std::size_t i = 0; i < sportCount; ++i) {
            os << sports.at(i) << (i + 1 < sportCount ? ", " : "");
        }
    }
    os << "\nCoaches (" << coaches.size() << "):\n";
    if (coaches.empty()) {
        os << "  None\n";
    }
    else {
        for (const auto& c : coaches) {
            os << "  - " << c.get_last_name() << " (" << c.get_sport()
                << ", Group: " << c.get_current_group_size() << "/" << c.get_max_group_size() << ")\n";
        }
    }
}

bool Facility::operator==(const Facility& other) const {
    return name == other.name;
}