#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include "Coach.h"

class Facility {
private:
    static constexpr int SPORTS_COUNT = 3;
    std::string name = "Unknown";
    int capacity = 0;
    std::array<std::string, SPORTS_COUNT> sports;
    std::size_t sportCount = 0;
    std::vector<Coach> coaches;

public:
    Facility() = default;
    Facility(std::string_view n, int cap);
    virtual ~Facility() = default;

    void add_sport(std::string_view sportName);
    bool has_sport(std::string_view sportName) const;
    void clear_and_reset_sports();
    void set_capacity(int newCap);
    void set_name(std::string_view newName);
    void print_short_info() const;

    bool add_coach(std::string_view lastName, std::string_view sport, int maxGroup);
    bool enroll_to_coach(std::string_view coachLastName, int people);
    void show_coaches() const;

    std::string get_name() const { return name; }
    int get_capacity() const { return capacity; }

    virtual std::string_view get_type_name() const = 0;
    virtual int calculate_max_daily_capacity() const = 0;
    virtual void print_full_info(std::ostream& os) const;

    bool operator==(const Facility& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Facility& f) {
        f.print_full_info(os);
        return os;
    }
};