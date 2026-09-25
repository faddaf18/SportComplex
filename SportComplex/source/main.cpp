#include <iostream>
#include "SportComplex.h"
#include "Facility.h"

int main() {
    SportComplex complex("Student Complex");

    Facility f1("Main Hall", "Indoor", 30);
    f1.add_sport("Football");
    f1.add_sport("Volleyball");
    complex += f1;

    int choice = 0;
    do {
        std::cout << "\n=== COMPLEX MENU ===\n";
        std::cout << "1. Short info \n";
        std::cout << "2. Full info\n";
        std::cout << "3. Edit facility\n";
        std::cout << "4. Check restrictions\n";
        std::cout << "5. Add new facility\n";
        std::cout << "6. Remove last facility\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            complex.show_all_short();
            break;
        case 2:
            complex.show_all_full();
            break;
        case 3:
            complex.edit_facility_menu();
            break;
        case 4:
            complex.try_enroll();
            break;
        case 5:
            complex.create_and_add_facility();
            break;
        case 6:
            --complex;
            std::cout << "Last facility removed.\n";
            break;
        case 0:
            std::cout << "Program terminated.\n";
            break;
        default:
            std::cout << "Invalid menu option.\n";
        }
    } while (choice != 0);

    return 0;
}