#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <compare>
#define SportsCount 3

class Facility {
private:
    std::string name = "Unknown";
    std::string type = "Unknown";
    int capacity = 0;
    std::array<std::string, SportsCount> sports;
    std::size_t sportCount = 0;

public:
    Facility() = default;
    Facility(std::string_view n, std::string_view t, int cap);

    void add_sport(std::string_view sportName);
    void clear_and_reset_sports();
    void set_capacity(int newCap);
    void set_name(std::string_view newName);
    void print_short_info() const;
    bool check_training(std::string_view sport, int people) const;

    std::string get_name() const { return name; }
    int get_capacity() const { return capacity; }

    bool operator==(const Facility& other) const;

    std::strong_ordering operator<=>(const Facility& other) const {
        return capacity <=> other.capacity;
    }

    friend std::ostream& operator<<(std::ostream& os, const Facility& f) {
        os << "Facility: " << f.name
            << "\nType: " << f.type
            << "\nCapacity: " << f.capacity << " people"
            << "\nSports: ";
        if (f.sportCount == 0) {
            os << "None";
        }
        else {
            for (std::size_t i = 0; i < f.sportCount; ++i) {
                os << f.sports.at(i) << (i + 1 < f.sportCount ? ", " : "");
            }
        }
        os << "\n-----------------------------\n";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Facility& f) {
        std::cout << "Enter facility name: ";
        std::getline(is >> std::ws, f.name);

        std::cout << "Enter facility type: ";
        std::getline(is, f.type);

        int cap = 0;
        std::cout << "Enter capacity: ";
        if (is >> cap) {
            f.set_capacity(cap);
        }
        else {
            f.set_capacity(10);
        }

        int sCount = 0;
        std::cout << "How many sports to add (0 to 3)? ";
        is >> sCount;
        is.ignore();

        f.sportCount = 0;
        for (int i = 0; i < sCount && i < SportsCount; ++i) {
            std::string sport;
            std::cout << "Sport #" << (i + 1) << ": ";
            std::getline(is, sport);
            f.add_sport(sport);
        }

        return is;
    }
};