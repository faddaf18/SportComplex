#include "Facility.h"

Facility::Facility(std::string_view n, int cap) : name(n) {
    set_capacity(cap);
}

void Facility::add_sport(std::string_view sportName) {
    if (static_cast<int>(sports.get_size()) < SPORTS_COUNT) {
        sports.add(std::string(sportName));
    }
    else {
        std::cout << "Sports limit reached for this facility (max 3).\n";
    }
}

bool Facility::has_sport(std::string_view sportName) const {
    return sports.find_if([&](const std::string& s) { return s == sportName; }) != nullptr;
}

bool Facility::remove_sport(std::size_t index) {
    return sports.remove(index);
}

void Facility::clear_and_reset_sports() {
    sports.clear();
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
    coaches.add(Coach(lastName, sport, maxGroup));
    std::cout << "-> SUCCESS: Coach " << lastName << " (" << sport << ") added to " << name << ".\n";
    return true;
}

bool Facility::enroll_to_coach(std::string_view coachLastName, int people) {
    Coach* coach = coaches.find_if([&](const Coach& c) {
        return c.get_last_name() == coachLastName;
        });
    if (coach == nullptr) {
        std::cout << "-> ERROR: Coach '" << coachLastName << "' not found in facility '" << name << "'!\n";
        return false;
    }
    if (coach->enroll(people)) {
        std::cout << "-> SUCCESS: Enrolled " << people << " athletes to coach "
            << coachLastName << ". Group status: "
            << coach->get_current_group_size() << "/" << coach->get_max_group_size() << ".\n";
        return true;
    }
    std::cout << "-> DENIED: Cannot enroll " << people << " athletes to coach "
        << coachLastName << ". Exceeds max group limit ("
        << coach->get_current_group_size() + people << "/" << coach->get_max_group_size() << ").\n";
    return false;
}

bool Facility::remove_coach(std::string_view lastName) {
    auto index = coaches.find_index([&](const Coach& c) {
        return c.get_last_name() == lastName;
        });
    if (!index.has_value()) {
        std::cout << "-> ERROR: Coach '" << lastName << "' not found in facility '" << name << "'!\n";
        return false;
    }
    coaches.remove(*index);
    std::cout << "-> SUCCESS: Coach " << lastName << " removed from " << name << ".\n";
    return true;
}

bool Facility::remove_coach_at(std::size_t index) {
    if (!coaches.remove(index)) {
        std::cout << "-> ERROR: Invalid coach number in facility '" << name << "'!\n";
        return false;
    }
    std::cout << "-> SUCCESS: Coach #" << index + 1 << " removed from " << name << ".\n";
    return true;
}

void Facility::clear_coaches() {
    coaches.clear();
    std::cout << "-> SUCCESS: All coaches removed from " << name << ".\n";
}

void Facility::show_coaches() const {
    if (coaches.empty()) {
        std::cout << "No coaches assigned to this facility.\n";
        return;
    }
    std::size_t number = 1;
    for (const auto& coach : coaches) {
        std::cout << number++ << ". ";
        coach.print_info();
    }
}

void Facility::print_full_info(std::ostream& os) const {
    os << "Facility: " << name
        << "\nType: " << get_type_name()
        << "\nCapacity: " << capacity << " people"
        << "\nMax Daily Capacity: " << calculate_max_daily_capacity() << " people"
        << "\nSports: ";
    if (sports.empty()) {
        os << "None";
    }
    else {
        for (std::size_t i = 0; i < sports.get_size(); ++i) {
            os << sports.get(i) << (i + 1 < sports.get_size() ? ", " : "");
        }
    }
    os << "\nCoaches (" << coaches.get_size() << "):\n";
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