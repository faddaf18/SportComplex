#include "SportComplex.h"
#include "Gym.h"
#include "SwimmingPool.h"
#include "Stadium.h"
#include "Collection.h"
#include <stdexcept>

static bool name_contains(const std::string_view& name, const std::string_view& part) {
#ifdef __cpp_lib_string_contains
    return name.contains(part);
#else
    return name.find(part) != std::string::npos;
#endif
}

SportComplex::SportComplex(std::string_view name) : complexName(name) {}

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

void SportComplex::create_and_add_facility() {
    if (halls.get_size() >= MAX_FACILITIES) {
        std::cout << "Cannot add more facilities, limit reached.\n";
        return;
    }

    std::cout << "Select facility type:\n";
    std::cout << "1. Gym\n";
    std::cout << "2. Swimming Pool\n";
    std::cout << "3. Stadium\n";
    std::cout << "Choice: ";
    int typeChoice = 0;
    std::cin >> typeChoice;
    std::cin.ignore();

    std::string name;
    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    int capacity = 0;
    std::cout << "Enter capacity: ";
    std::cin >> capacity;

    std::unique_ptr<Facility> newFacility;

    if (typeChoice == 1) {
        int equipCount = 0;
        std::cout << "Enter equipment count: ";
        std::cin >> equipCount;
        newFacility = std::make_unique<Gym>(name, capacity, equipCount);
    }
    else if (typeChoice == 2) {
        int lanes = 0;
        std::cout << "Enter lane count: ";
        std::cin >> lanes;
        newFacility = std::make_unique<SwimmingPool>(name, capacity, lanes);
    }
    else if (typeChoice == 3) {
        int lightChoice = 0;
        std::cout << "Has lighting? (1 - Yes, 0 - No): ";
        std::cin >> lightChoice;
        newFacility = std::make_unique<Stadium>(name, capacity, lightChoice != 0);
    }
    else {
        std::cout << "Invalid choice.\n";
        return;
    }

    int sCount = 0;
    std::cout << "How many sports to add (0 to 3)? ";
    std::cin >> sCount;
    std::cin.ignore();

    for (int i = 0; i < sCount && i < 3; ++i) {
        std::string sport;
        std::cout << "Sport #" << (i + 1) << ": ";
        std::getline(std::cin, sport);
        newFacility->add_sport(sport);
    }

    *this += std::move(newFacility);
    std::cout << "Facility successfully added to complex!\n";
}

void SportComplex::edit_facility_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Enter facility number to edit (or 0 to cancel): ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= halls.get_size()) {
        std::cout << "1. Change name\n";
        std::cout << "2. Change capacity\n";
        std::cout << "3. Change sports\n";
        std::cout << "Choice: ";
        int editChoice = 0;
        std::cin >> editChoice;
        if (editChoice == 1) {
            std::cout << "Enter new name: ";
            std::string newName;
            std::cin.ignore();
            std::getline(std::cin, newName);
            halls.get(index - 1)->set_name(newName);
            std::cout << "Name successfully updated!\n";
        }
        else if (editChoice == 2) {
            std::cout << "Enter new capacity: ";
            int newCap = 0;
            std::cin >> newCap;
            halls.get(index - 1)->set_capacity(newCap);
            std::cout << "Capacity successfully updated!\n";
        }
        else if (editChoice == 3) {
            halls.get(index - 1)->clear_and_reset_sports();
            std::cout << "Sports successfully updated!\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::add_coach_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Select facility number to assign a coach: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= halls.get_size()) {
        std::cin.ignore();
        std::string lastName;
        std::string sport;
        int maxGroup = 0;
        std::cout << "Enter coach last name: ";
        std::getline(std::cin, lastName);
        std::cout << "Enter coach sport specialization: ";
        std::getline(std::cin, sport);
        std::cout << "Enter coach max group size: ";
        std::cin >> maxGroup;

        halls.get(index - 1)->add_coach(lastName, sport, maxGroup);
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::enroll_to_coach_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Select facility number: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= halls.get_size()) {
        std::cout << "\nCoaches in this facility:\n";
        halls.get(index - 1)->show_coaches();
        std::cin.ignore();
        std::string coachName;
        int people = 0;
        std::cout << "Enter coach last name: ";
        std::getline(std::cin, coachName);
        std::cout << "Enter number of athletes to enroll: ";
        std::cin >> people;

        halls.get(index - 1)->enroll_to_coach(coachName, people);
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::remove_coach_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Select facility number: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= halls.get_size()) {
        std::cout << "\nCoaches in this facility:\n";
        halls.get(index - 1)->show_coaches();
        std::cout << "1. Remove coach by number\n";
        std::cout << "2. Remove coach by last name\n";
        std::cout << "3. Remove all coaches (clear)\n";
        std::cout << "Choice: ";
        int choice = 0;
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "Enter coach number: ";
            std::size_t coachIndex = 0;
            std::cin >> coachIndex;
            if (coachIndex == 0) {
                std::cout << "-> ERROR: Invalid coach number!\n";
            }
            else {
                halls.get(index - 1)->remove_coach_at(coachIndex - 1);
            }
        }
        else if (choice == 2) {
            std::cin.ignore();
            std::string lastName;
            std::cout << "Enter coach last name: ";
            std::getline(std::cin, lastName);
            halls.get(index - 1)->remove_coach(lastName);
        }
        else if (choice == 3) {
            halls.get(index - 1)->clear_coaches();
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::show_facility_by_index_menu() const {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Enter facility number (or 0 to cancel): ";
    std::size_t index = 0;
    std::cin >> index;
    if (index == 0) return;
    try {
        std::cout << "\n" << *halls.get(index - 1);
    }
    catch (const std::out_of_range&) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::remove_facility_by_index_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Enter facility number to remove (or 0 to cancel): ";
    std::size_t index = 0;
    std::cin >> index;
    if (index == 0) return;
    if (halls.remove(index - 1)) {
        std::cout << "Facility successfully removed!\n";
    }
    else {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::search_facilities_menu() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "1. Find facilities by name (part of name)\n";
    std::cout << "2. Find facilities by sport\n";
    std::cout << "3. Find facilities with capacity >= N\n";
    std::cout << "4. Find facilities by type (1 - Gym, 2 - Swimming Pool, 3 - Stadium)\n";
    std::cout << "Choice: ";
    int choice = 0;
    std::cin >> choice;

    Collection<std::size_t> found;
    if (choice == 1 || choice == 2) {
        std::cin.ignore();
        std::string text;
        std::cout << (choice == 1 ? "Enter name: " : "Enter sport: ");
        std::getline(std::cin, text);
        if (choice == 1) {
            found = select_indices(halls, [text](const std::unique_ptr<Facility>& f) {
                return name_contains(f->get_name(), text);
                });
        }
        else {
            found = select_indices(halls, [text](const std::unique_ptr<Facility>& f) {
                return f->has_sport(text);
                });
        }
    }
    else if (choice == 3) {
        int minCap = 0;
        std::cout << "Enter N: ";
        std::cin >> minCap;
        found = select_indices(halls, [minCap](const std::unique_ptr<Facility>& f) {
            return f->get_capacity() >= minCap;
            });
    }
    else if (choice == 4) {
        int type = 0;
        std::cout << "Type: ";
        std::cin >> type;
        found = select_indices(halls, [type](const std::unique_ptr<Facility>& f) {
            switch (type) {
            case 1: return dynamic_cast<const Gym*>(f.get()) != nullptr;
            case 2: return dynamic_cast<const SwimmingPool*>(f.get()) != nullptr;
            case 3: return dynamic_cast<const Stadium*>(f.get()) != nullptr;
            default: return false;
            }
            });
    }
    else {
        std::cout << "Invalid choice.\n";
        return;
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

void SportComplex::clear_facilities_menu() {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "Remove ALL " << halls.get_size() << " facilities? (1 - Yes, 0 - No): ";
    int confirm = 0;
    std::cin >> confirm;
    if (confirm == 1) {
        halls.clear();
        std::cout << "Complex cleared. Facilities count: " << halls.get_size() << "\n";
    }
    else {
        std::cout << "Cancelled.\n";
    }
}

void SportComplex::show_coach_by_index_menu() const {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Select facility number: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= halls.get_size()) {
        const Facility& hall = *halls.get(index - 1);
        std::cout << "\nCoaches in this facility (" << hall.get_coaches().get_size() << "):\n";
        hall.show_coaches();
        if (hall.get_coaches().empty()) return;
        std::cout << "Enter coach number: ";
        std::size_t coachIndex = 0;
        std::cin >> coachIndex;
        if (coachIndex == 0) {
            std::cout << "Invalid coach number.\n";
            return;
        }
        try {
            std::cout << hall.get_coaches().get(coachIndex - 1) << "\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "Invalid coach number.\n";
        }
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

void SportComplex::manage_sports_menu() {
    show_all_short();
    if (halls.empty()) return;
    std::cout << "Select facility number: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index == 0) return;
    if (index > halls.get_size()) {
        std::cout << "Invalid facility number.\n";
        return;
    }
    Facility& hall = *halls.get(index - 1);

    std::cout << "\nSports of '" << hall.get_name() << "' (" << hall.get_sports().get_size() << "):\n"
        << hall.get_sports();
    std::cout << "1. Get sport by number\n";
    std::cout << "2. Add sport\n";
    std::cout << "3. Remove sport by number\n";
    std::cout << "4. Check if sport is available\n";
    std::cout << "5. Remove all sports (clear)\n";
    std::cout << "Choice: ";
    int choice = 0;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Enter sport number: ";
        std::size_t sportIndex = 0;
        std::cin >> sportIndex;
        try {
            if (sportIndex == 0) throw std::out_of_range("zero");
            std::cout << "Sport #" << sportIndex << ": " << hall.get_sports().get(sportIndex - 1) << "\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "Invalid sport number.\n";
        }
    }
    else if (choice == 2) {
        std::cin.ignore();
        std::string sport;
        std::cout << "Enter sport: ";
        std::getline(std::cin, sport);
        hall.add_sport(sport);
    }
    else if (choice == 3) {
        std::cout << "Enter sport number: ";
        std::size_t sportIndex = 0;
        std::cin >> sportIndex;
        if (sportIndex > 0 && hall.remove_sport(sportIndex - 1)) {
            std::cout << "Sport removed.\n";
        }
        else {
            std::cout << "Invalid sport number.\n";
        }
    }
    else if (choice == 4) {
        std::cin.ignore();
        std::string sport;
        std::cout << "Enter sport: ";
        std::getline(std::cin, sport);
        std::cout << (hall.has_sport(sport) ? "Available.\n" : "Not available.\n");
    }
    else if (choice == 5) {
        hall.clear_sports();
        std::cout << "All sports removed.\n";
    }
    else {
        std::cout << "Invalid choice.\n";
    }
}

void SportComplex::search_coaches_menu() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "1. Find coaches by sport\n";
    std::cout << "2. Find coaches with max group size >= N\n";
    std::cout << "3. Show coach with the largest current group\n";
    std::cout << "Choice: ";
    int choice = 0;
    std::cin >> choice;

    if (choice == 1 || choice == 2) {
        std::string sport;
        int minGroup = 0;
        if (choice == 1) {
            std::cin.ignore();
            std::cout << "Enter sport: ";
            std::getline(std::cin, sport);
        }
        else {
            std::cout << "Enter N: ";
            std::cin >> minGroup;
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
        std::cout << "Invalid choice.\n";
    }
}

SportComplex& SportComplex::operator+=(std::unique_ptr<Facility> f) {
    if (halls.get_size() < MAX_FACILITIES) {
        halls.add(std::move(f));
    }
    else {
        std::cout << "Error: Limit reached (max 10 facilities) in '" << complexName << "'!\n";
    }
    return *this;
}

SportComplex& SportComplex::operator--() {
    if (!halls.empty()) {
        halls.remove(halls.get_size() - 1);
    }
    else {
        std::cout << "Error: No facilities to remove in '" << complexName << "'!\n";
    }
    return *this;
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