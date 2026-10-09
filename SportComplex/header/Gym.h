#pragma once
#include "Facility.h"

class Gym : public Facility {
private:
    int equipmentCount = 0;

public:
    Gym() = default;
    Gym(std::string_view n, int cap, int equipCount);

    void set_equipment_count(int count);
    int get_equipment_count() const { return equipmentCount; }
    int calculate_required_instructors() const;

    std::string_view get_type_name() const override { return "Gym"; }
    int calculate_max_daily_capacity() const override;
    void print_full_info(std::ostream& os) const override;
};