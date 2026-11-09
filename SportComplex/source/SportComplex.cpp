#include "SportComplex.h"
#include "Gym.h"
#include "SwimmingPool.h"
#include "Stadium.h"
#include "Collection.h"
#include "SportComplexException.h"
#include "InvalidDataException.h"
#include "ObjectNotFoundException.h"
#include "DuplicateIdException.h"
#include "ConstraintViolationException.h"
#include "InvalidOperationException.h"
#include "IndexOutOfRangeException.h"
#include "BrokenRelationException.h"
#include "Input.h"

using input::read_int;
using input::read_line;
using input::read_number;

static bool name_contains(const std::string& name, const std::string& part) {
#ifdef __cpp_lib_string_contains
    return name.contains(part);
#else
    return name.find(part) != std::string::npos;
#endif
}

SportComplex::SportComplex(std::string_view name) : complexName(name) {}

Facility& SportComplex::facility_at(std::size_t number) {
    if (number == 0 || number > halls.get_size()) {
        throw IndexOutOfRangeException("Facility", number, halls.get_size());
    }
    return *halls.get(number - 1);
}

const Facility& SportComplex::facility_at(std::size_t number) const {
    if (number == 0 || number > halls.get_size()) {
        throw IndexOutOfRangeException("Facility", number, halls.get_size());
    }
    return *halls.get(number - 1);
}

void SportComplex::ensure_name_unique(const std::string& name) const {
    const auto* same = halls.find_if([&name](const std::unique_ptr<Facility>& f) {
        return f->get_name() == name;
        });
    if (same != nullptr) {
        throw DuplicateIdException("Facility", name, "complex '" + complexName + "'");
    }
}

void SportComplex::report_current_exception() const {
    try {
        throw;
    }
    catch (const InvalidDataException& e) {
        std::cout << "[INVALID DATA] " << e.what() << "\n"
            << "Check the value of '" << e.get_field() << "' and try again.\n";
    }
    catch (const ObjectNotFoundException& e) {
        std::cout << "[NOT FOUND] " << e.what() << "\n";
        std::cout << "Check the spelling of '" << e.get_key() << "' (the name is case-sensitive).\n";
    }
    catch (const DuplicateIdException& e) {
        std::cout << "[DUPLICATE] " << e.what() << "\n"
            << "Identifiers must be unique: choose a name other than '" << e.get_id() << "'.\n";
    }
    catch (const ConstraintViolationException& e) {
        std::cout << "[LIMIT VIOLATED] " << e.what() << "\n"
            << "Violated constraint: " << e.get_constraint() << ". Nothing was changed.\n";
    }
    catch (const IndexOutOfRangeException& e) {
        std::cout << "[OUT OF RANGE] " << e.what() << "\n";
        if (e.get_count() > 0) {
            std::cout << "Enter a number between 1 and " << e.get_count() << ".\n";
        }
    }
    catch (const BrokenRelationException& e) {
        std::cout << "[BROKEN LINK] " << e.what() << "\n"
            << "The operation was cancelled, existing data is intact.\n";
    }
    catch (const InvalidOperationException& e) {
        std::cout << "[NOT ALLOWED] " << e.what() << "\n";
    }
    catch (const SportComplexException& e) {
        std::cout << "[ERROR] " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "[UNEXPECTED ERROR] " << e.what() << "\n";
    }
}

void SportComplex::show_all_short() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "\n=== Facilities List ('" << complexName << "') ===\n";
    for (std::size_t i = 0; i < halls.get_size(); i++) {
        std::cout << i + 1 << ". ";
        halls.get(i)->print_short_info();
    }
}

void SportComplex::show_all_full() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "\n=== Full Facilities Information ===\n";
    for (const auto& hall : halls) {
        std::cout << *hall;
    }
}

static std::unique_ptr<Facility> create_facility_of_type(int typeChoice, const std::string& name, int capacity) {
    if (typeChoice == 1) {
        int equipCount = read_int("Enter equipment count: ", "equipment count");
        return std::make_unique<Gym>(name, capacity, equipCount);
    }
    if (typeChoice == 2) {
        int lanes = read_int("Enter lane count: ", "lane count");
        return std::make_unique<SwimmingPool>(name, capacity, lanes);
    }
    int lightChoice = read_int("Has lighting? (1 - Yes, 0 - No): ", "lighting");
    return std::make_unique<Stadium>(name, capacity, lightChoice != 0);
}

void SportComplex::create_and_add_facility() {
    try {
        if (halls.get_size() >= MAX_FACILITIES) {
            throw ConstraintViolationException("max facilities",
                "Cannot add more facilities, limit reached (" + std::to_string(MAX_FACILITIES) + ").");
        }

        std::cout << "Select facility type:\n";
        std::cout << "1. Gym\n";
        std::cout << "2. Swimming Pool\n";
        std::cout << "3. Stadium\n";
        int typeChoice = read_int("Choice: ", "facility type");
        if (typeChoice < 1 || typeChoice > 3) {
            throw InvalidDataException("facility type", "choose 1, 2 or 3 (got " + std::to_string(typeChoice) + ")");
        }

        std::string name = read_line("Enter name: ", "facility name");
        ensure_name_unique(name);
        int capacity = read_int("Enter capacity: ", "capacity");

        std::unique_ptr<Facility> newFacility = create_facility_of_type(typeChoice, name, capacity);

        int sCount = read_int("How many sports to add (0 to 3)? ", "sports count");
        if (sCount < 0 || sCount > 3) {
            throw InvalidDataException("sports count", "must be between 0 and 3 (got " + std::to_string(sCount) + ")");
        }
        for (int i = 0; i < sCount; ++i) {
            std::string sport = read_line("Sport #" + std::to_string(i + 1) + ": ", "sport");
            newFacility->add_sport(sport);
        }

        *this += std::move(newFacility);
        std::cout << "Facility successfully added to complex!\n";
    }
    catch (const std::exception&) {
        report_current_exception();
        std::cout << "Facility was not added.\n";
    }
}

void SportComplex::edit_facility_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Enter facility number to edit (or 0 to cancel): ", "facility number");
        if (number == 0) return;
        Facility& facility = facility_at(number);

        std::cout << "1. Change name\n";
        std::cout << "2. Change capacity\n";
        std::cout << "3. Change sports\n";
        int editChoice = read_int("Choice: ", "edit choice");
        if (editChoice == 1) {
            std::string newName = read_line("Enter new name: ", "facility name");
            if (newName != facility.get_name()) {
                ensure_name_unique(newName);
            }
            facility.set_name(newName);
            std::cout << "Name successfully updated!\n";
        }
        else if (editChoice == 2) {
            int newCap = read_int("Enter new capacity: ", "capacity");
            facility.set_capacity(newCap);
            std::cout << "Capacity successfully updated!\n";
        }
        else if (editChoice == 3) {
            facility.clear_and_reset_sports();
            std::cout << "Sports successfully updated!\n";
        }
        else {
            throw InvalidDataException("edit choice", "choose 1, 2 or 3 (got " + std::to_string(editChoice) + ")");
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::add_coach_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Select facility number to assign a coach: ", "facility number");
        if (number == 0) return;
        Facility& facility = facility_at(number);

        std::string lastName = read_line("Enter coach last name: ", "coach last name");
        std::string sport = read_line("Enter coach sport specialization: ", "coach sport");
        int maxGroup = read_int("Enter coach max group size: ", "coach max group size");

        facility.add_coach(lastName, sport, maxGroup);
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::enroll_to_coach_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Select facility number: ", "facility number");
        if (number == 0) return;
        Facility& facility = facility_at(number);

        std::cout << "\nCoaches in this facility:\n";
        facility.show_coaches();
        std::string coachName = read_line("Enter coach last name: ", "coach last name");
        int people = read_int("Enter number of athletes to enroll: ", "number of athletes");

        facility.enroll_to_coach(coachName, people);
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::show_facility_by_index_menu() const {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Enter facility number (or 0 to cancel): ", "facility number");
        if (number == 0) return;
        std::cout << "\n" << facility_at(number);
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::remove_facility_by_index_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Enter facility number to remove (or 0 to cancel): ", "facility number");
        if (number == 0) return;
        facility_at(number); 
        halls.remove(number - 1);
        std::cout << "Facility successfully removed!\n";
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::clear_facilities_menu() {
    try {
        if (halls.empty()) {
            throw InvalidOperationException("the complex '" + complexName + "' has no facilities to remove.");
        }
        std::string question = "Remove ALL " + std::to_string(halls.get_size()) + " facilities? (1 - Yes, 0 - No): ";
        int confirm = read_int(question, "confirmation");
        if (confirm == 1) {
            halls.clear();
            std::cout << "Complex cleared. Facilities count: " << halls.get_size() << "\n";
        }
        else {
            std::cout << "Cancelled.\n";
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::search_facilities_menu() const {
    try {
        if (halls.empty()) {
            std::cout << "No facilities in the complex.\n";
            return;
        }
        std::cout << "1. Find facilities by name (part of name)\n";
        std::cout << "2. Find facilities by sport\n";
        std::cout << "3. Find facilities with capacity >= N\n";
        std::cout << "4. Find facilities by type (1 - Gym, 2 - Swimming Pool, 3 - Stadium)\n";
        int choice = read_int("Choice: ", "search choice");

        Collection<std::size_t> found;
        if (choice == 1) {
            std::string text = read_line("Enter name: ", "name");
            found = select_indices(halls, [text](const std::unique_ptr<Facility>& f) {
                return name_contains(f->get_name(), text);
                });
        }
        else if (choice == 2) {
            std::string text = read_line("Enter sport: ", "sport");
            found = select_indices(halls, [text](const std::unique_ptr<Facility>& f) {
                return f->has_sport(text);
                });
        }
        else if (choice == 3) {
            int minCap = read_int("Enter N: ", "capacity");
            found = select_indices(halls, [minCap](const std::unique_ptr<Facility>& f) {
                return f->get_capacity() >= minCap;
                });
        }
        else if (choice == 4) {
            int type = read_int("Type: ", "facility type");
            if (type < 1 || type > 3) {
                throw InvalidDataException("facility type", "choose 1, 2 or 3 (got " + std::to_string(type) + ")");
            }
            found = select_indices(halls, [type](const std::unique_ptr<Facility>& f) {
                switch (type) {
                case 1: return dynamic_cast<const Gym*>(f.get()) != nullptr;
                case 2: return dynamic_cast<const SwimmingPool*>(f.get()) != nullptr;
                default: return dynamic_cast<const Stadium*>(f.get()) != nullptr;
                }
                });
        }
        else {
            throw InvalidDataException("search choice", "choose 1-4 (got " + std::to_string(choice) + ")");
        }

        if (found.empty()) {
            std::cout << "Nothing found.\n";
            return;
        }
        std::cout << "\nFound facilities: " << found.get_size() << "\n";
        for (std::size_t i = 0; i < found.get_size(); ++i) {
            std::cout << found.get(i) + 1 << ". ";
            halls.get(found.get(i))->print_short_info();
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::search_coaches_menu() const {
    try {
        if (halls.empty()) {
            std::cout << "No facilities in the complex.\n";
            return;
        }
        std::cout << "1. Find coaches by sport\n";
        std::cout << "2. Find coaches with max group size >= N\n";
        std::cout << "3. Show coach with the largest current group\n";
        int choice = read_int("Choice: ", "search choice");

        if (choice == 1 || choice == 2) {
            std::string sport;
            int minGroup = 0;
            if (choice == 1) {
                sport = read_line("Enter sport: ", "sport");
            }
            else {
                minGroup = read_int("Enter N: ", "group size");
            }

            std::size_t total = 0;
            for (const auto& hall : halls) {
                Collection<Coach> found = (choice == 1)
                    ? filter_collection(hall->get_coaches(), [&sport](const Coach& c) { return c.get_sport() == sport; })
                    : filter_collection(hall->get_coaches(), [minGroup](const Coach& c) { return c.get_max_group_size() >= minGroup; });
                if (!found.empty()) {
                    std::cout << "[" << hall->get_name() << "]\n" << found;
                    total += found.get_size();
                }
            }
            std::cout << "Total coaches found: " << total << "\n";
        }
        else if (choice == 3) {
            Collection<Coach> all;
            for (const auto& hall : halls) {
                for (const auto& coach : hall->get_coaches()) {
                    all.add(coach);
                }
            }
            const Coach* best = find_max_by(all, [](const Coach& c) { return c.get_current_group_size(); });
            if (best == nullptr) {
                std::cout << "No coaches in the complex.\n";
            }
            else {
                std::cout << "Largest group: " << *best << "\n";
            }
        }
        else {
            throw InvalidDataException("search choice", "choose 1-3 (got " + std::to_string(choice) + ")");
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::show_coach_by_index_menu() const {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Select facility number: ", "facility number");
        if (number == 0) return;
        const Facility& facility = facility_at(number);

        std::cout << "\nCoaches in this facility (" << facility.get_coaches().get_size() << "):\n";
        facility.show_coaches();
        if (facility.get_coaches().empty()) return;
        std::size_t coachNumber = read_number("Enter coach number: ", "coach number");
        std::cout << facility.get_coach_at(coachNumber) << "\n";
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::remove_coach_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Select facility number: ", "facility number");
        if (number == 0) return;
        Facility& facility = facility_at(number);

        std::cout << "\nCoaches in this facility:\n";
        facility.show_coaches();
        std::cout << "1. Remove coach by number\n";
        std::cout << "2. Remove coach by last name\n";
        std::cout << "3. Remove all coaches (clear)\n";
        int choice = read_int("Choice: ", "remove choice");
        if (choice == 1) {
            std::size_t coachNumber = read_number("Enter coach number: ", "coach number");
            facility.remove_coach_at(coachNumber);
        }
        else if (choice == 2) {
            std::string lastName = read_line("Enter coach last name: ", "coach last name");
            facility.remove_coach(lastName);
        }
        else if (choice == 3) {
            facility.clear_coaches();
        }
        else {
            throw InvalidDataException("remove choice", "choose 1-3 (got " + std::to_string(choice) + ")");
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

void SportComplex::manage_sports_menu() {
    try {
        show_all_short();
        if (halls.empty()) return;
        std::size_t number = read_number("Select facility number: ", "facility number");
        if (number == 0) return;
        Facility& facility = facility_at(number);

        std::cout << "\nSports of '" << facility.get_name() << "' (" << facility.get_sports().get_size() << "):\n"
            << facility.get_sports();
        std::cout << "1. Get sport by number\n";
        std::cout << "2. Add sport\n";
        std::cout << "3. Remove sport by number\n";
        std::cout << "4. Check if sport is available\n";
        std::cout << "5. Remove all sports (clear)\n";
        int choice = read_int("Choice: ", "sports choice");

        if (choice == 1) {
            std::size_t sportNumber = read_number("Enter sport number: ", "sport number");
            std::cout << "Sport #" << sportNumber << ": " << facility.get_sport_at(sportNumber) << "\n";
        }
        else if (choice == 2) {
            std::string sport = read_line("Enter sport: ", "sport");
            facility.add_sport(sport);
            std::cout << "Sport added.\n";
        }
        else if (choice == 3) {
            std::size_t sportNumber = read_number("Enter sport number: ", "sport number");
            facility.remove_sport(sportNumber);
            std::cout << "Sport removed.\n";
        }
        else if (choice == 4) {
            std::string sport = read_line("Enter sport: ", "sport");
            std::cout << (facility.has_sport(sport) ? "Available.\n" : "Not available.\n");
        }
        else if (choice == 5) {
            facility.clear_sports();
            std::cout << "All sports removed.\n";
        }
        else {
            throw InvalidDataException("sports choice", "choose 1-5 (got " + std::to_string(choice) + ")");
        }
    }
    catch (const std::exception&) {
        report_current_exception();
    }
}

SportComplex& SportComplex::operator+=(std::unique_ptr<Facility> f) {
    if (!f) {
        throw InvalidDataException("facility", "an empty (null) facility cannot be added");
    }
    if (halls.get_size() >= MAX_FACILITIES) {
        throw ConstraintViolationException("max facilities",
            "Limit reached (max " + std::to_string(MAX_FACILITIES) + " facilities) in '" + complexName + "'!");
    }
    ensure_name_unique(f->get_name());
    halls.add(std::move(f));
    return *this;
}

SportComplex& SportComplex::operator--() {
    if (halls.empty()) {
        throw InvalidOperationException("no facilities to remove in '" + complexName + "'!");
    }
    halls.remove(halls.get_size() - 1);
    return *this;
}

void SportComplex::show_statistics() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    Collection<std::string> names = transform_collection(halls, [](const std::unique_ptr<Facility>& f) {
        return f->get_name();
        });
    Collection<int> daily = transform_collection(halls, [](const std::unique_ptr<Facility>& f) {
        return f->calculate_max_daily_capacity();
        });
    Collection<Coach> allCoaches;
    for (const auto& hall : halls) {
        for (const auto& coach : hall->get_coaches()) {
            allCoaches.add(coach);
        }
    }

    int totalDaily = 0;
    for (int value : daily) totalDaily += value;
    const auto* biggest = find_max_by(halls, [](const std::unique_ptr<Facility>& f) {
        return f->get_capacity();
        });

    std::cout << "\n=== STATISTICS ('" << complexName << "') ===\n";
    std::cout << "Facilities count (Collection<unique_ptr<Facility>>): " << halls.get_size() << "\n";
    std::cout << "Facility names (Collection<string>):\n" << names;
    std::cout << "Total max daily capacity (Collection<int>): " << totalDaily << " people\n";
    std::cout << "Gyms: " << count_matching(halls, [](const std::unique_ptr<Facility>& f) {
        return dynamic_cast<const Gym*>(f.get()) != nullptr; })
        << ", Swimming pools: " << count_matching(halls, [](const std::unique_ptr<Facility>& f) {
        return dynamic_cast<const SwimmingPool*>(f.get()) != nullptr; })
        << ", Stadiums: " << count_matching(halls, [](const std::unique_ptr<Facility>& f) {
        return dynamic_cast<const Stadium*>(f.get()) != nullptr; }) << "\n";
    std::cout << "Largest facility by capacity: " << (*biggest)->get_name() << "\n";
    std::cout << "All coaches (Collection<Coach>), total " << allCoaches.get_size() << ":\n" << allCoaches;
}

void SportComplex::show_specific_features(std::ostream& os) const {
    if (halls.empty()) {
        os << "No facilities in the complex.\n";
        return;
    }

    os << "\n=== SPECIFIC FACILITY FEATURES ===\n";
    for (size_t i = 0; i < halls.get_size(); ++i) {
        os << i + 1 << ". [" << halls[i]->get_name() << "] (" << halls[i]->get_type_name() << "):\n";

        if (const auto* gym = dynamic_cast<const Gym*>(halls[i].get())) {
            os << "   -> Required instructors for equipment: " << gym->calculate_required_instructors() << "\n";
        }
        else if (const auto* pool = dynamic_cast<const SwimmingPool*>(halls[i].get())) {
            os << "   -> Required water volume: " << pool->calculate_water_volume() << " liters\n";
        }
        else if (const auto* stadium = dynamic_cast<const Stadium*>(halls[i].get())) {
            os << "   -> Maintenance cost: " << stadium->calculate_maintenance_cost() << " USD\n";
        }
    }
    os << "==================================\n";
}
