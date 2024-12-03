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
const double L = 2.0;  // Длина стороны квадратной области

// Класс для представления заряда
class Charge {
public:
    std::array<double, 2> position;

    // Конструктор: случайная генерация положения заряда в пределах области
    Charge();

    // Вычисление силы взаимодействия с другим зарядом
    std::array<double, 2> calculate_force(const Charge& other) const;

    // Генерация случайного числа в диапазоне [min, max]
    double random_double(double min, double max) const;
};

// Класс для симуляции системы зарядов
class Simulation {
public:
    std::vector<Charge> charges;           // Заряды в системе
    std::vector<std::array<double, 2>> m;  // Первый момент (средний градиент) для Adam
    std::vector<std::array<double, 2>> v;  // Второй момент (средний квадрат градиента) для Adam

    // Конструктор: инициализация системы зарядов
    Simulation(int n);

    // Вычисление энергии системы зарядов
    double calculate_energy() const;

    // Обновление позиций зарядов для минимизации энергии (методом Adam)
    void update_positions();

    // Вывод позиций зарядов
    void print_positions() const;

private:
    // Adam гиперпараметры
    const double beta1;      // Коэффициент сглаживания для первого момента
    const double beta2;      // Коэффициент сглаживания для второго момента
    const double epsilon;    // Малое число для предотвращения деления на 0
    const double learning_rate; // Скорость обучения (шаг оптимизации)
};

#endif //INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
