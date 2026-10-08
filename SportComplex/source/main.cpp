#include <iostream>
#include <memory>
#include "SportComplex.h"
#include "Gym.h"
#include "SwimmingPool.h"
#include "Stadium.h"

int main() {
    SportComplex complex("Student Complex");

    auto gym = std::make_unique<Gym>("Power Gym", 30, 15);
    gym->add_sport("Fitness");
    gym->add_sport("Powerlifting");
    gym->add_coach("Smith", "Fitness", 15);
    complex += std::move(gym);

    auto pool = std::make_unique<SwimmingPool>("Blue Wave", 50, 6);
    pool->add_sport("Swimming");
    pool->add_sport("Water Polo");
    pool->add_coach("Johnson", "Swimming", 12);
    complex += std::move(pool);

    auto stadium = std::make_unique<Stadium>("Central Arena", 200, true);
    stadium->add_sport("Football");
    stadium->add_sport("Athletics");
    stadium->add_coach("Williams", "Football", 20);
    complex += std::move(stadium);

    int choice = 0;
    do {
        std::cout << "\n=== COMPLEX MENU ===\n";
        std::cout << "1. Short info\n";
        std::cout << "2. Full info\n";
        std::cout << "3. Add new coach to facility\n";
        std::cout << "4. Enroll athletes to coach\n";
        std::cout << "5. Edit facility\n";
        std::cout << "6. Add new facility\n";
        std::cout << "7. Remove last facility\n";
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
            complex.add_coach_menu();
            break;
        case 4:
            complex.enroll_to_coach_menu();
            break;
        case 5:
            complex.edit_facility_menu();
            break;
        case 6:
            complex.create_and_add_facility();
            break;
        case 7:
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