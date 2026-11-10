#include "Facility.h"
#include "Input.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/DuplicateIdException.h"
#include "exceptions/ConstraintViolationException.h"
#include "exceptions/BrokenRelationException.h"
#include "exceptions/InvalidOperationException.h"
#include "exceptions/IndexOutOfRangeException.h"
#include "exceptions/ObjectNotFoundException.h"
#include <format>

Facility::Facility(std::string_view n, int cap) : name(n) {
    if (n.empty()) {
        throw InvalidDataException("facility name", "must not be empty");
    }
    set_capacity(cap);
}

void Facility::add_sport(std::string_view sportName) {
    if (sportName.empty()) {
        throw InvalidDataException("sport", "name must not be empty");
    }
    if (has_sport(sportName)) {
        throw DuplicateIdException("Sport", std::string(sportName), std::format("facility '{}'", name));
    }
    if (static_cast<int>(sports.get_size()) >= SPORTS_COUNT) {
        throw ConstraintViolationException("sports limit",
            std::format("Sports limit reached for facility '{}' (max {}).", name, SPORTS_COUNT));
    }
    sports.add(std::string(sportName));
}

bool Facility::has_sport(std::string_view sportName) const {
    return sports.find_if([sportName](const std::string& s) { return s == sportName; }) != nullptr;
}

void Facility::remove_sport(std::size_t number) {
    if (number == 0 || number > sports.get_size()) {
        throw IndexOutOfRangeException("Sport", number, sports.get_size());
    }
    const std::string sportName = sports.get(number - 1);
    if (const Coach* user = coaches.find_if([&sportName](const Coach& c) { return c.get_sport() == sportName; }); user != nullptr) {
        throw BrokenRelationException(std::format("coach {} works in sport '{}' of facility '{}'.",
            user->get_last_name(), sportName, name));
    }
    sports.remove(number - 1);
}

void Facility::clear_sports() {
    if (sports.empty()) {
        throw InvalidOperationException(std::format("facility '{}' has no sports to remove.", name));
    }
    if (!coaches.empty()) {
        throw BrokenRelationException(std::format("facility '{}' still has {} coach(es) linked to its sports.",
            name, coaches.get_size()));
    }
    sports.clear();
}

const std::string& Facility::get_sport_at(std::size_t number) const {
    if (number == 0 || number > sports.get_size()) {
        throw IndexOutOfRangeException("Sport", number, sports.get_size());
    }
    return sports.get(number - 1);
}

void Facility::clear_and_reset_sports() {
    int sCount = input::read_int("How many sports will be available (1 to 3)? ", "sports count");
    if (sCount < 1 || sCount > SPORTS_COUNT) {
        throw InvalidDataException("sports count", std::format("must be between 1 and {} (got {})", SPORTS_COUNT, sCount));
    }

    Collection<std::string> newSports;
    for (int i = 0; i < sCount; i++) {
        std::string sport = input::read_line(std::format("Enter sport #{}: ", i + 1), "sport");
        if (sport.empty()) {
            throw InvalidDataException("sport", "name must not be empty");
        }
        if (newSports.find_if([&sport](const std::string_view& s) { return s == sport; }) != nullptr) {
            throw DuplicateIdException("Sport", sport, "the new sports list");
        }
        newSports.add(sport);
    }
    for (const auto& coach : coaches) {
        const std::string coachSport(coach.get_sport());
        if (newSports.find_if([&coachSport](const std::string_view& s) { return s == coachSport; }) == nullptr) {
            throw BrokenRelationException(std::format("coach {} works in sport '{}' which is absent from the new list.",
                coach.get_last_name(), coachSport));
        }
    }
    sports = std::move(newSports);
}

void Facility::set_capacity(int newCap) {
    if (newCap <= 0) {
        throw InvalidDataException("capacity", std::format("must be greater than 0 (got {})", newCap));
    }
    if (const Coach* tooBig = coaches.find_if([newCap](const Coach& c) { return c.get_max_group_size() > newCap; }); tooBig != nullptr) {
        throw ConstraintViolationException("capacity",
            std::format("Capacity {} is less than the group size of coach {} ({}).",
                newCap, tooBig->get_last_name(), tooBig->get_max_group_size()));
    }
    capacity = newCap;
}

void Facility::set_name(std::string_view newName) {
    if (newName.empty()) {
        throw InvalidDataException("facility name", "must not be empty");
    }
    name = newName;
}

void Facility::print_short_info() const {
    std::cout << std::format("Facility: {} ({}) | Capacity: {} people\n", name, get_type_name(), capacity);
}

void Facility::add_coach(std::string_view lastName, std::string_view sport, int maxGroup) {
    Coach newCoach(lastName, sport, maxGroup);
    if (coaches.find_if([lastName](const Coach& c) { return c.get_last_name() == lastName; }) != nullptr) {
        throw DuplicateIdException("Coach", std::string(lastName), std::format("facility '{}'", name));
    }
    if (!has_sport(sport)) {
        throw BrokenRelationException(std::format("facility '{}' does not support sport '{}', so coach {} cannot be linked to it.",
            name, sport, lastName));
    }
    if (maxGroup > capacity) {
        throw ConstraintViolationException("facility capacity",
            std::format("Coach group size ({}) exceeds facility capacity ({}).", maxGroup, capacity));
    }
    coaches.add(std::move(newCoach));
    std::cout << std::format("-> SUCCESS: Coach {} ({}) added to {}.\n", lastName, sport, name);
}

void Facility::enroll_to_coach(std::string_view coachLastName, int people) {
    Coach* coach = coaches.find_if([coachLastName](const Coach& c) {
        return c.get_last_name() == coachLastName;
        });
    if (coach == nullptr) {
        throw ObjectNotFoundException("Coach", std::string(coachLastName), std::format("facility '{}'", name));
    }
    coach->enroll(people);
    std::cout << std::format("-> SUCCESS: Enrolled {} athletes to coach {}. Group status: {}/{}.\n",
        people, coachLastName, coach->get_current_group_size(), coach->get_max_group_size());
}

void Facility::remove_coach(std::string_view lastName) {
    auto index = coaches.find_index([lastName](const Coach& c) {
        return c.get_last_name() == lastName;
        });
    if (!index.has_value()) {
        throw ObjectNotFoundException("Coach", std::string(lastName), std::format("facility '{}'", name));
    }
    coaches.remove(*index);
    std::cout << std::format("-> SUCCESS: Coach {} removed from {}.\n", lastName, name);
}

void Facility::remove_coach_at(std::size_t number) {
    if (number == 0 || number > coaches.get_size()) {
        throw IndexOutOfRangeException("Coach", number, coaches.get_size());
    }
    coaches.remove(number - 1);
    std::cout << std::format("-> SUCCESS: Coach #{} removed from {}.\n", number, name);
}

void Facility::clear_coaches() {
    if (coaches.empty()) {
        throw InvalidOperationException(std::format("facility '{}' has no coaches to remove.", name));
    }
    coaches.clear();
    std::cout << std::format("-> SUCCESS: All coaches removed from {}.\n", name);
}

const Coach& Facility::get_coach_at(std::size_t number) const {
    if (number == 0 || number > coaches.get_size()) {
        throw IndexOutOfRangeException("Coach", number, coaches.get_size());
    }
    return coaches.get(number - 1);
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
    os << std::format("Facility: {}\nType: {}\nCapacity: {} people\nMax Daily Capacity: {} people\nSports: ",
        name, get_type_name(), capacity, calculate_max_daily_capacity());
    if (sports.empty()) {
        os << "None";
    }
    else {
        for (std::size_t i = 0; i < sports.get_size(); ++i) {
            os << sports.get(i) << (i + 1 < sports.get_size() ? ", " : "");
        }
    }
    os << std::format("\nCoaches ({}):\n", coaches.get_size());
    if (coaches.empty()) {
        os << "  None\n";
    }
    else {
        for (const auto& c : coaches) {
            os << std::format("  - {} ({}, Group: {}/{})\n",
                c.get_last_name(), c.get_sport(), c.get_current_group_size(), c.get_max_group_size());
        }
    }
}

bool Facility::operator==(const Facility& other) const {
    return name == other.name;
}