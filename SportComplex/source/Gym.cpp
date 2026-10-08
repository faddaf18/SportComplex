#include "Gym.h"

Gym::Gym(std::string_view n, int cap, int equipCount)
    : Facility(n, cap), equipmentCount(equipCount > 0 ? equipCount : 0) {}

void Gym::set_equipment_count(int count) {
    equipmentCount = count > 0 ? count : 0;
}

int Gym::calculate_max_daily_capacity() const {
    return get_capacity() * 8;
}

void Gym::print_full_info(std::ostream& os) const {
    Facility::print_full_info(os);
    os << "Equipment count: " << equipmentCount
        << "\n-----------------------------\n";
}