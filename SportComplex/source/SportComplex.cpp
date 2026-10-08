#include "SportComplex.h"
#include "Gym.h"
#include "SwimmingPool.h"
#include "Stadium.h"

SportComplex::SportComplex(std::string_view name) : complexName(name) {}

void SportComplex::show_all_short() const {
    if (halls.empty()) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "\n=== Facilities List ('" << complexName << "') ===\n";
    for (std::size_t i = 0; i < halls.size(); i++) {
        std::cout << i + 1 << ". ";
        halls.at(i)->print_short_info();
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
    if (halls.size() >= MAX_FACILITIES) {
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
    if (index > 0 && index <= halls.size()) {
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
            halls.at(index - 1)->set_name(newName);
            std::cout << "Name successfully updated!\n";
        }
        else if (editChoice == 2) {
            std::cout << "Enter new capacity: ";
            int newCap = 0;
            std::cin >> newCap;
            halls.at(index - 1)->set_capacity(newCap);
            std::cout << "Capacity successfully updated!\n";
        }
        else if (editChoice == 3) {
            halls.at(index - 1)->clear_and_reset_sports();
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
    if (index > 0 && index <= halls.size()) {
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

        halls.at(index - 1)->add_coach(lastName, sport, maxGroup);
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
    if (index > 0 && index <= halls.size()) {
        std::cout << "\nCoaches in this facility:\n";
        halls.at(index - 1)->show_coaches();
        std::cin.ignore();
        std::string coachName;
        int people = 0;
        std::cout << "Enter coach last name: ";
        std::getline(std::cin, coachName);
        std::cout << "Enter number of athletes to enroll: ";
        std::cin >> people;

        halls.at(index - 1)->enroll_to_coach(coachName, people);
    }
    else if (index != 0) {
        std::cout << "Invalid facility number.\n";
    }
}

SportComplex& SportComplex::operator+=(std::unique_ptr<Facility> f) {
    if (halls.size() < MAX_FACILITIES) {
        halls.push_back(std::move(f));
    }
    else {
        std::cout << "Error: Limit reached (max 10 facilities) in '" << complexName << "'!\n";
    }
    return *this;
}

SportComplex& SportComplex::operator--() {
    if (!halls.empty()) {
        halls.pop_back();
    }
    else {
        std::cout << "Error: No facilities to remove in '" << complexName << "'!\n";
    }
    return *this;
}