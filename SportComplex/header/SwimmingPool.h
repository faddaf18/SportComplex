#pragma once
#include "Facility.h"

class SwimmingPool : public Facility {
private:
    int laneCount = 0;

public:
    SwimmingPool() = default;
    SwimmingPool(std::string_view n, int cap, int lanes);

    void set_lane_count(int lanes);
    int get_lane_count() const { return laneCount; }
    int calculate_water_volume() const;

    std::string_view get_type_name() const override { return "Swimming Pool"; }
    int calculate_max_daily_capacity() const override;
    void print_full_info(std::ostream& os) const override;
    
};