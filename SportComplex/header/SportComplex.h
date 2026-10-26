#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "Facility.h"
#include "Collection.h"

class SportComplex {
private:
    static constexpr std::size_t MAX_FACILITIES = 10;
    std::string complexName;
    Collection<std::unique_ptr<Facility>> halls;

public:
    explicit SportComplex(std::string_view name);
    void show_all_short() const;
    void show_all_full() const;
    void create_and_add_facility();
    void edit_facility_menu();
    void add_coach_menu();
    void enroll_to_coach_menu();
    void show_facility_by_index_menu() const;
    void remove_facility_by_index_menu();
    void search_facilities_menu() const;
    void show_statistics() const;
    void clear_facilities_menu();
    void show_coach_by_index_menu() const;
    void manage_sports_menu();
    void remove_coach_menu();
    void search_coaches_menu() const;
    void show_specific_features(std::ostream& os) const;

    std::size_t get_halls_count() const { return halls.get_size(); }

    SportComplex& operator+=(std::unique_ptr<Facility> f);
    SportComplex& operator--();
};