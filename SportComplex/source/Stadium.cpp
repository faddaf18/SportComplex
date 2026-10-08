#include "Stadium.h"

Stadium::Stadium(std::string_view n, int cap, bool lighting)
    : Facility(n, cap), hasLighting(lighting) {}

int Stadium::calculate_max_daily_capacity() const {
    return get_capacity() * (hasLighting ? 3 : 1);
}

void Stadium::print_full_info(std::ostream& os) const {
    Facility::print_full_info(os);
    os << "Has lighting: " << (hasLighting ? "Yes" : "No")
        << "\n-----------------------------\n";
}