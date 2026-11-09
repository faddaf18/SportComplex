#pragma once
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>
#include "InvalidDataException.h"

namespace input {

    inline void skip_line() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    inline int read_int(const std::string& prompt, const std::string& field) {
        std::cout << prompt;
        int value = 0;
        if (!(std::cin >> value)) {
            if (std::cin.eof()) {
                throw InvalidDataException(field, "input stream was closed");
            }
            std::cin.clear();
            skip_line();
            throw InvalidDataException(field, "an integer number is expected");
        }
        skip_line();
        return value;
    }

    inline std::size_t read_number(const std::string& prompt, const std::string& field) {
        int value = read_int(prompt, field);
        if (value < 0) {
            throw InvalidDataException(field, "must not be negative (got " + std::to_string(value) + ")");
        }
        return static_cast<std::size_t>(value);
    }

    inline std::string read_line(const std::string& prompt, const std::string& field) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            throw InvalidDataException(field, "input stream was closed");
        }
        return line;
    }
}

