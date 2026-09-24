#include "SportComplex.h"

void SportComplex::add_facility(const Facility& f) {
    *this += f; 
}

void SportComplex::show_all_short() const {
    if (hallsCount == 0) {
        std::cout << "В комплексе пока нет залов.\n";
        return;
    }
    std::cout << "\n=== Список объектов комплекса '" << complexName << "' (Кратко) ===\n";
    for (std::size_t i = 0; i < hallsCount; i++) {
        std::cout << i + 1 << ". ";
        halls.at(i).print_short_info();
    }
}

void SportComplex::show_all_full() const {
    if (hallsCount == 0) {
        std::cout << "В комплексе пока нет залов.\n";
        return;
    }
    std::cout << "\n=== Полная информация объектов комплекса ===\n";
    for (std::size_t i = 0; i < hallsCount; i++) {
        halls.at(i).print_full_info();
    }
}

void SportComplex::create_and_add_facility() {
    if (hallsCount >= 10) {
        std::cout << "Нельзя добавить больше залов, лимит исчерпан.\n";
        return;
    }

    Facility newFacility;
    std::cin >> newFacility; 

    *this += newFacility;
}

void SportComplex::edit_facility_menu() {
    show_all_short();
    if (hallsCount == 0) return;

    std::cout << "Введите номер зала для редактирования (или 0 для отмены): ";
    std::size_t index = 0;
    std::cin >> index;

    if (index > 0 && index <= hallsCount) {
        std::cout << "1. Изменить название\n";
        std::cout << "2. Изменить вместимость\n";
        std::cout << "3. Изменить виды спорта\n";
        std::cout << "Выбор: ";
        int editChoice = 0;
        std::cin >> editChoice;

        if (editChoice == 1) {
            std::cout << "Введите новое название: ";
            std::string newName;
            std::cin.ignore();
            std::getline(std::cin, newName);
            halls.at(index - 1).set_name(newName);
            std::cout << "Название успешно изменено!\n";
        }
        else if (editChoice == 2) {
            std::cout << "Введите новую вместимость: ";
            int newCap = 0;
            std::cin >> newCap;
            halls.at(index - 1).set_capacity(newCap);
            std::cout << "Вместимость успешно изменена!\n";
        }
        else if (editChoice == 3) {
            halls.at(index - 1).clear_and_reset_sports();
            std::cout << "Виды спорта успешно обновлены!\n";
        }
        else {
            std::cout << "Неверный выбор действия.\n";
        }
    }
    else if (index != 0) {
        std::cout << "Неверный номер зала.\n";
    }
}

void SportComplex::try_enroll() const {
    show_all_short();
    if (hallsCount == 0) return;

    std::cout << "Выберите номер зала для тренировки: ";
    std::size_t index = 0;
    std::cin >> index;

    if (index > 0 && index <= hallsCount) {
        std::cin.ignore();
        std::cout << "Какой вид спорта планируется? ";
        std::string sport;
        std::getline(std::cin, sport);

        std::cout << "Сколько человек будет в группе? ";
        int people = 0;
        std::cin >> people;

        bool result = halls.at(index - 1).check_training(sport, people);

        if (result == true) {
            std::cout << "-> УСПЕХ: Все условия соблюдены, тренировка разрешена!\n";
        }
    }
    else if (index != 0) {
        std::cout << "Неверный номер зала!\n";
    }
}

SportComplex& SportComplex::operator+=(const Facility& f) {
    if (hallsCount < 10) {
        halls.at(hallsCount) = f;
        hallsCount++;
        std::cout << "Объект '" << f.get_name() << "' успешно добавлен в комплекс через оператор += !\n";
    }
    else {
        std::cout << "Ошибка [operator+=]: В комплексе '" << complexName
            << "' достигнут лимит залов (максимум 10)! Добавление невыполнимо.\n";
    }
    return *this;
}

SportComplex& SportComplex::operator--() {
    if (hallsCount > 0) {
        hallsCount--;
        std::cout << "Последний зал удален из комплекса через оператор --. Осталось залов: " << hallsCount << "\n";
    }
    else {
        std::cout << "Ошибка [operator--]: В комплексе '" << complexName
            << "' нет залов для удаления! Операция невыполнима.\n";
    }
    return *this;
}

SportComplex SportComplex::operator--(int) {
    SportComplex temp = *this;
    --(*this);
    return temp;
}