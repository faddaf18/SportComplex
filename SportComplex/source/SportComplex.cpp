#include "SportComplex.h"

SportComplex::SportComplex(std::string_view name) : complexName(name) {}

void SportComplex::show_all_short() const {
    if (hallsCount == 0) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "\n=== Facilities List ('" << complexName << "') ===\n";
    for (std::size_t i = 0; i < hallsCount; i++) {
        std::cout << i + 1 << ". ";
        halls.at(i).print_short_info();
    }
}

void SportComplex::show_all_full() const {
    if (hallsCount == 0) {
        std::cout << "No facilities in the complex.\n";
        return;
    }
    std::cout << "\n=== Full Facilities Information ===\n";
    for (std::size_t i = 0; i < hallsCount; i++) {
        std::cout << halls.at(i);
    }
}

void SportComplex::create_and_add_facility() {
    if (hallsCount >= FACILITYCOUNT) {
        std::cout << "Cannot add more facilities, limit reached.\n";
        return;
    }

    Facility newFacility;
    std::cin >> newFacility;
    *this += newFacility;
    std::cout << "Facility successfully added to complex!\n";
}

void SportComplex::edit_facility_menu() {
    show_all_short();
    if (hallsCount == 0) return;
    std::cout << "Enter facility number to edit (or 0 to cancel): ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= hallsCount) {
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
            halls.at(index - 1).set_name(newName);
            std::cout << "Name successfully updated!\n";
        }
        else if (editChoice == 2) {
            std::cout << "Enter new capacity: ";
            int newCap = 0;
            std::cin >> newCap;
            halls.at(index - 1).set_capacity(newCap);
            std::cout << "Capacity successfully updated!\n";
        }
        else if (editChoice == 3) {
            halls.at(index - 1).clear_and_reset_sports();
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

void SportComplex::try_enroll() const {
    show_all_short();
    if (hallsCount == 0) return;
    std::cout << "Select facility number for training: ";
    std::size_t index = 0;
    std::cin >> index;
    if (index > 0 && index <= hallsCount) {
        std::cin.ignore();
        std::cout << "Which sport is planned? ";
        std::string sport;
        std::getline(std::cin, sport);
        std::cout << "How many people in the group? ";
        int people = 0;
        std::cin >> people;
        bool result = halls.at(index - 1).check_training(sport, people);
        if (result) {
            std::cout << "-> SUCCESS: All conditions met, training approved!\n";
        }
    }
    else if (index != 0) {
        std::cout << "Invalid facility number!\n";
    }
}

SportComplex& SportComplex::operator+=(const Facility& f) {
    if (hallsCount < FACILITYCOUNT) {
        halls.at(hallsCount) = f;
        hallsCount++;
    }
    else {
        std::cout << "Error: Limit reached (max 10 facilities) in '" << complexName << "'!\n";
    }
    return *this;
}

SportComplex& SportComplex::operator--() {
    if (hallsCount > 0) {
        hallsCount--;
    }
    else {
        std::cout << "Error: No facilities to remove in '" << complexName << "'!\n";
    }
    return *this;
}