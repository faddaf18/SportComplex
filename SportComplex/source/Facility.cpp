#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include "Facility.h"

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