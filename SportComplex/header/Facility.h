#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <compare>

class Facility {
private:
    std::string name = "Unknown";
    std::string type = "Unknown";
    int capacity = 0;
    std::array<std::string, 3> sports;
    std::size_t sportCount = 0;

public:
    Facility() = default;
    Facility(std::string_view n, std::string_view t, int cap);

    void init(std::string_view n, std::string_view t, int cap);
    void add_sport(std::string_view sportName);
    void clear_and_reset_sports();
    void set_capacity(int newCap);
    void set_name(std::string_view newName);
    std::string get_name() const { return name; }
    int get_capacity() const { return capacity; }
    void print_short_info() const;
    void print_full_info() const;
    bool check_training(std::string_view sport, int people) const;
    bool operator==(const Facility& other) const;
    bool operator!=(const Facility& other) const;
    std::strong_ordering operator<=>(const Facility& other) const { return capacity <=> other.capacity; }

    friend std::ostream& operator<<(std::ostream& os, const Facility& f);
    friend std::istream& operator>>(std::istream& is, Facility& f);
    friend bool shareSameSports(const Facility& f1, const Facility& f2);
    friend void inspectFacilityInternals(const Facility& f);
};