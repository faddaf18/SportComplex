#pragma once
#include "Facility.h"

class Stadium : public Facility {
private:
    bool hasLighting = false;

public:
    Stadium() = default;
    Stadium(std::string_view n, int cap, bool lighting);

    void set_lighting(bool lighting) { hasLighting = lighting; }
    bool get_lighting() const { return hasLighting; }
    std::string_view check_lighting_readiness() const;

    std::string_view get_type_name() const override { return "Stadium"; }
    int calculate_max_daily_capacity() const override;
    void print_full_info(std::ostream& os) const override;
};