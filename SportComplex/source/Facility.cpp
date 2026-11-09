#include "Facility.h"
#include "Input.h"
#include "InvalidDataException.h"
#include "DuplicateIdException.h"
#include "ConstraintViolationException.h"
#include "BrokenRelationException.h"
#include "InvalidOperationException.h"
#include "IndexOutOfRangeException.h"
#include "ObjectNotFoundException.h"

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
        throw DuplicateIdException("Sport", std::string(sportName), "facility '" + name + "'");
    }
    if (static_cast<int>(sports.get_size()) >= SPORTS_COUNT) {
        throw ConstraintViolationException("sports limit",
            "Sports limit reached for facility '" + name + "' (max " + std::to_string(SPORTS_COUNT) + ").");
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
        throw BrokenRelationException("coach " + std::string(user->get_last_name()) +
            " works in sport '" + sportName + "' of facility '" + name + "'.");
    }
    sports.remove(number - 1);
}

void Facility::clear_sports() {
    if (sports.empty()) {
        throw InvalidOperationException("facility '" + name + "' has no sports to remove.");
    }
    if (!coaches.empty()) {
        throw BrokenRelationException("facility '" + name + "' still has " +
            std::to_string(coaches.get_size()) + " coach(es) linked to its sports.");
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
        throw InvalidDataException("sports count", "must be between 1 and " + std::to_string(SPORTS_COUNT) +
            " (got " + std::to_string(sCount) + ")");
    }

    Collection<std::string> newSports;
    for (int i = 0; i < sCount; i++) {
        std::string sport = input::read_line("Enter sport #" + std::to_string(i + 1) + ": ", "sport");
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
            throw BrokenRelationException("coach " + std::string(coach.get_last_name()) +
                " works in sport '" + coachSport + "' which is absent from the new list.");
        }
    }
    sports = std::move(newSports);
}

void Facility::set_capacity(int newCap) {
    if (newCap <= 0) {
        throw InvalidDataException("capacity", "must be greater than 0 (got " + std::to_string(newCap) + ")");
    }
    if (const Coach* tooBig = coaches.find_if([newCap](const Coach& c) { return c.get_max_group_size() > newCap; }); tooBig != nullptr) {
        throw ConstraintViolationException("capacity",
            "Capacity " + std::to_string(newCap) + " is less than the group size of coach " +
            std::string(tooBig->get_last_name()) + " (" + std::to_string(tooBig->get_max_group_size()) + ").");
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
    std::cout << "Facility: " << name << " (" << get_type_name() << ") | Capacity: " << capacity << " people\n";
}

void Facility::add_coach(std::string_view lastName, std::string_view sport, int maxGroup) {
    Coach newCoach(lastName, sport, maxGroup);
    if (coaches.find_if([lastName](const Coach& c) { return c.get_last_name() == lastName; }) != nullptr) {
        throw DuplicateIdException("Coach", std::string(lastName), "facility '" + name + "'");
    }
    if (!has_sport(sport)) {
        throw BrokenRelationException("facility '" + name + "' does not support sport '" +
            std::string(sport) + "', so coach " + std::string(lastName) + " cannot be linked to it.");
    }
    if (maxGroup > capacity) {
        throw ConstraintViolationException("facility capacity",
            "Coach group size (" + std::to_string(maxGroup) + ") exceeds facility capacity (" +
            std::to_string(capacity) + ").");
    }
    coaches.add(std::move(newCoach));
    std::cout << "-> SUCCESS: Coach " << lastName << " (" << sport << ") added to " << name << ".\n";
}

void Facility::enroll_to_coach(std::string_view coachLastName, int people) {
    Coach* coach = coaches.find_if([coachLastName](const Coach& c) {
        return c.get_last_name() == coachLastName;
        });
    if (coach == nullptr) {
        throw ObjectNotFoundException("Coach", std::string(coachLastName), "facility '" + name + "'");
    }
    coach->enroll(people);
    std::cout << "-> SUCCESS: Enrolled " << people << " athletes to coach "
        << coachLastName << ". Group status: "
        << coach->get_current_group_size() << "/" << coach->get_max_group_size() << ".\n";
}

void Facility::remove_coach(std::string_view lastName) {
    auto index = coaches.find_index([lastName](const Coach& c) {
        return c.get_last_name() == lastName;
        });
    if (!index.has_value()) {
        throw ObjectNotFoundException("Coach", std::string(lastName), "facility '" + name + "'");
    }
    coaches.remove(*index);
    std::cout << "-> SUCCESS: Coach " << lastName << " removed from " << name << ".\n";
}

void Facility::remove_coach_at(std::size_t number) {
    if (number == 0 || number > coaches.get_size()) {
        throw IndexOutOfRangeException("Coach", number, coaches.get_size());
    }
    coaches.remove(number - 1);
    std::cout << "-> SUCCESS: Coach #" << number << " removed from " << name << ".\n";
}

void Facility::clear_coaches() {
    if (coaches.empty()) {
        throw InvalidOperationException("facility '" + name + "' has no coaches to remove.");
    }
    coaches.clear();
    std::cout << "-> SUCCESS: All coaches removed from " << name << ".\n";
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