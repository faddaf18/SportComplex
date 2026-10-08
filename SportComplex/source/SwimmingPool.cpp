#include "SwimmingPool.h"

SwimmingPool::SwimmingPool(std::string_view n, int cap, int lanes)
    : Facility(n, cap), laneCount(lanes > 0 ? lanes : 0) {}

void SwimmingPool::set_lane_count(int lanes) {
    laneCount = lanes > 0 ? lanes : 0;
}

int SwimmingPool::calculate_max_daily_capacity() const {
    return laneCount * 20;
}

void SwimmingPool::print_full_info(std::ostream& os) const {
    Facility::print_full_info(os);
    os << "Lanes: " << laneCount
        << "\n-----------------------------\n";
}