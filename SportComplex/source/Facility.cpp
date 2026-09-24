#include "Facility.h"

Facility::Facility(std::string_view n, std::string_view t, int cap) {
    init(n, t, cap);
}

void Facility::init(std::string_view n, std::string_view t, int cap) {
    name = n;
    type = t;
    set_capacity(cap);
    sportCount = 0;
}

void Facility::add_sport(std::string_view sportName) {
    if (sportCount < 3) {
        sports.at(sportCount) = std::string(sportName);
        sportCount++;
    }
    else {
        std::cout << "Достигнут лимит видов спорта для этого зала (максимум 3).\n";
    }
}

void Facility::clear_and_reset_sports() {
    sportCount = 0;
    std::cout << "Сколько видов спорта будет доступно (от 1 до 3)? ";
    int sCount = 0;
    std::cin >> sCount;
    std::cin.ignore();

    for (int i = 0; i < sCount && i < 3; i++) {
        std::string sport;
        std::cout << "Введите вид спорта #" << (i + 1) << ": ";
        std::getline(std::cin, sport);
        add_sport(sport);
    }
}

void Facility::set_capacity(int newCap) {
    if (newCap > 0) {
        capacity = newCap;
    }
    else {
        std::cout << "Ошибка: Вместимость должна быть больше 0! Установлено значение по умолчанию: 10.\n";
        capacity = 10;
    }
}

void Facility::set_name(std::string_view newName) {
    name = newName;
}

void Facility::print_short_info() const {
    std::cout << "Объект: " << name << " (" << type << ") | Вместимость: " << capacity << " чел.\n";
}

void Facility::print_full_info() const {
    std::cout << " Информация об объекте \n";
    std::cout << "Название: " << name << "\n";
    std::cout << "Тип: " << type << "\n";
    std::cout << "Вместимость: " << capacity << " человек\n";
    std::cout << "Доступные виды спорта: ";
    if (sportCount == 0) {
        std::cout << "пока нет";
    }
    else {
        for (std::size_t i = 0; i < sportCount; i++) {
            std::cout << sports.at(i) << " ";
        }
    }
    std::cout << "\n-----------------------------\n";
}

bool Facility::check_training(std::string_view sport, int people) const {
    if (people > capacity) {
        std::cout << "-> Отказ: Группа из " << people << " человек превышает вместимость зала (" << capacity << " мест).\n";
        return false;
    }

    bool hasSport = false;
    for (std::size_t i = 0; i < sportCount; i++) {
        if (sports.at(i) == sport) {
            hasSport = true;
            break;
        }
    }

    if (hasSport == false) {
        std::cout << "-> Отказ: В зале '" << name << "' не поддерживается вид спорта: " << sport << ".\n";
        return false;
    }

    return true;
}

bool Facility::operator==(const Facility& other) const {
    return name == other.name;
}

bool Facility::operator!=(const Facility& other) const {
    return !(*this == other);
}

bool Facility::operator<(const Facility& other) const {
    return capacity < other.capacity;
}

bool Facility::operator>(const Facility& other) const {
    return capacity > other.capacity;
}

bool Facility::operator<=(const Facility& other) const {
    return capacity <= other.capacity;
}

bool Facility::operator>=(const Facility& other) const {
    return capacity >= other.capacity;
}

std::ostream& operator<<(std::ostream& os, const Facility& f) {
    os << "[Facility] Название: " << f.name
        << " | Тип: " << f.type
        << " | Вместимость: " << f.capacity
        << " | Виды спорта (" << f.sportCount << "/3): ";
    if (f.sportCount == 0) {
        os << "нет";
    }
    else {
        for (std::size_t i = 0; i < f.sportCount; ++i) {
            os << f.sports.at(i) << (i + 1 < f.sportCount ? ", " : "");
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, Facility& f) {
    std::cout << "Введите название объекта: ";
    std::getline(is >> std::ws, f.name);

    std::cout << "Введите тип объекта: ";
    std::getline(is, f.type);

    int cap = 0;
    std::cout << "Введите вместимость: ";
    if (is >> cap) {
        f.set_capacity(cap);
    }
    else {
        f.set_capacity(10);
    }

    int sCount = 0;
    std::cout << "Сколько видов спорта добавить (от 0 до 3)? ";
    is >> sCount;
    is.ignore();

    f.sportCount = 0;
    for (int i = 0; i < sCount && i < 3; ++i) {
        std::string sport;
        std::cout << "Вид спорта #" << (i + 1) << ": ";
        std::getline(is, sport);
        f.add_sport(sport);
    }

    return is;
}

bool shareSameSports(const Facility& f1, const Facility& f2) {
    for (std::size_t i = 0; i < f1.sportCount; ++i) {
        for (std::size_t j = 0; j < f2.sportCount; ++j) {
            if (f1.sports.at(i) == f2.sports.at(j)) {
                return true;
            }
        }
    }
    return false;
}

void inspectFacilityInternals(const Facility& f) {
    std::cout << "[INSPECT PRIVATE DATA] Зал '" << f.name
        << "', Поле capacity=" << f.capacity
        << ", Занято слотов в sports[]=" << f.sportCount << ":\n";
    for (std::size_t i = 0; i < f.sportCount; ++i) {
        std::cout << "  - sports[" << i << "] = " << f.sports.at(i) << "\n";
    }
}