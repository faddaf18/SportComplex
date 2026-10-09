#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "Facility.h"

class SportComplex {
private:
    static constexpr std::size_t MAX_FACILITIES = 10;
    std::string complexName;
    std::vector<std::unique_ptr<Facility>> halls;

public:
    explicit SportComplex(std::string_view name);
    void show_all_short() const;
    void show_all_full() const;
    void create_and_add_facility();
    void edit_facility_menu();
    void add_coach_menu();
    void enroll_to_coach_menu();
    void show_specific_features(std::ostream& os) const;

    std::size_t get_halls_count() const { return halls.size(); }

    SportComplex& operator+=(std::unique_ptr<Facility> f);
    SportComplex& operator--();
};