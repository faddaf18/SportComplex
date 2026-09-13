#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include "SportComplex.h"
#include "Facility.h"

int main() {
    std::setlocale(LC_ALL, ".65001");

    SportComplex complex("Sport Complex");

    Facility f1;
    f1.init("Main court", "Covered", 30);
    f1.add_sport("Football");
    f1.add_sport("Volleyball");
    complex.add_facility(f1);

    int choice = 0;
    do {
        std::cout << "\n=== SPORT COMPLEX ===\n";
        std::cout << "1. Short ifno \n";
        std::cout << "2. Full info\n";
        std::cout << "3. Make adjustments\n";
        std::cout << "4. Sign up for a training session\n";
        std::cout << "5. Add a court\n";
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
        case 0:
            std::cout << "Программа завершена.\n";
            break;
        default:
            std::cout << "Неверный пункт меню.\n";
        }
    } while (choice != 0);

    return 0;
}