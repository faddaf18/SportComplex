#pragma once
#include <cstddef>
#include <string>
#include "SportComplexException.h"

class IndexOutOfRangeException : public SportComplexException {
private:
    std::size_t number;
    std::size_t count;

    static std::string build_message(const std::string& target, std::size_t num, std::size_t cnt) {
        std::string range = (cnt == 0) ? "the list is empty" : "valid numbers: 1.." + std::to_string(cnt);
        return target + " number " + std::to_string(num) + " is out of range (" + range + ").";
    }

public:
    IndexOutOfRangeException(const std::string& target, std::size_t num, std::size_t cnt)
        : SportComplexException(build_message(target, num, cnt)), number(num), count(cnt) {}

    std::size_t get_number() const { return number; }
    std::size_t get_count() const { return count; }
};

