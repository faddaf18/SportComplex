#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include "Facility.h"

class SportComplex {
private:
    std::string complexName;
    std::array<Facility, 10> halls;
    std::size_t hallsCount = 0;

public:
    explicit SportComplex(std::string_view name) : complexName(name) {}
    void add_facility(const Facility& f);
    void show_all_short() const;
    void show_all_full() const;
    void create_and_add_facility();
    void edit_facility_menu();
    void try_enroll() const;
};