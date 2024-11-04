//
// Created by MaxV on 04.11.2024.
//

#ifndef INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
#define INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H

#include <array>
#include <random>
#include <cmath>
#include <vector>
#include <iostream>

// Константы
const double k = 1.0;  // электростатическая постоянная
const double e = 1.0;  // заряд
const double R = 1.0;  // радиус сферы

// Класс для представления заряда
class Charge {
public:
    std::array<double, 2> position;

    // Конструктор: случайная генерация положения заряда на сфере
    Charge();

    // Вычисление силы взаимодействия с другим зарядом
    std::array<double, 2> calculate_force(const Charge& other) const;

private:
    // Генерация случайного числа в диапазоне [min, max]
    double random_double(double min, double max) const;
};

// Класс для симуляции системы зарядов
class Simulation {
public:
    std::vector<Charge> charges;

    // Конструктор: инициализация системы зарядов
    Simulation(int n);

    // Вычисление энергии системы зарядов
    double calculate_energy() const;

    // Обновление позиций зарядов для минимизации энергии
    void update_positions();

    // Вывод позиций зарядов
    void print_positions() const;
};


#endif //INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
