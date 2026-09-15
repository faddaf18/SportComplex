#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>

class Facility {
private:
    std::string name = "Unknown";
    std::string type = "Unknown";
    int capacity = 0;
    std::array<std::string, 3> sports;
    std::size_t sportCount = 0;

public:
    Facility() = default;
    void init(std::string_view n, std::string_view t, int cap);
    void add_sport(std::string_view sportName);
    void clear_and_reset_sports();
    void set_capacity(int newCap);
    void set_name(std::string_view newName);
    void print_short_info() const;
    void print_full_info() const;
    bool check_training(std::string_view sport, int people) const;
};