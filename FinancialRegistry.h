#pragma once
#ifndef FINANCIAL_REGISTRY_H_
#define FINANCIAL_REGISTRY_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ПУНКТ 5: Шаблон класу з 5 шаблонними полями даних та шаблонними методами
template <typename T>
class FinancialRegistry {
private:
    // 5 шаблонних полів даних:
    std::vector<T> items_;       // 1. Динамічний масив шаблонних елементів
    T primaryItem_;              // 2. Основний (еталонний) елемент реєстру
    T reserveItem_;              // 3. Резервний елемент
    T minThreshold_;             // 4. Мінімальний пороговий об'єкт
    T maxThreshold_;             // 5. Максимальний пороговий об'єкт
    std::string registryTitle_;

public:
    // Конструктор за замовчуванням
    FinancialRegistry()
        : primaryItem_(T()),
        reserveItem_(T()),
        minThreshold_(T()),
        maxThreshold_(T()),
        registryTitle_("Загальний реєстр") {
    }

    // Параметризований конструктор
    FinancialRegistry(const std::string& title, const T& primary,
        const T& reserve, const T& minVal, const T& maxVal)
        : primaryItem_(primary),
        reserveItem_(reserve),
        minThreshold_(minVal),
        maxThreshold_(maxVal),
        registryTitle_(title) {
    }

    // 1. Шаблонний метод додавання елемента до реєстру
    void addItem(const T& item) { items_.push_back(item); }

    // 2. Шаблонний метод отримання елемента за індексом
    T getItem(size_t index) const {
        if (index >= items_.size()) {
            throw std::out_of_range("Індекс виходить за межі шаблонного реєстру!");
        }
        return items_[index];
    }

    // 3. Шаблонний метод заміни основного елемента (повертає попередній)
    T swapPrimaryItem(const T& newPrimary) {
        T old = primaryItem_;
        primaryItem_ = newPrimary;
        return old;
    }

    // 4. Шаблонний метод оновлення порогових значень
    void setThresholds(const T& minVal, const T& maxVal) {
        minThreshold_ = minVal;
        maxThreshold_ = maxVal;
    }

    // 5. Шаблонний метод пошуку суми всіх елементів (працює для типів з оператором +)
    T calculateTotalSum() const {
        T total = T();
        for (const auto& item : items_) {
            total = total + item;
        }
        return total;
    }

    // 6. Шаблонний метод виведення інформації про реєстр
    void displayRegistry() const {
        std::cout << "=== Реєстр: " << registryTitle_
            << " (елементів: " << items_.size() << ") ===\n";
        std::cout << "Основний: " << primaryItem_ << " | Резервний: " << reserveItem_
            << "\n";
        std::cout << "Пороги: [" << minThreshold_ << " .. " << maxThreshold_
            << "]\n";
        for (size_t i = 0; i < items_.size(); ++i) {
            std::cout << "  [" << i << "] -> " << items_[i] << "\n";
        }
    }
};

#endif  // FINANCIAL_REGISTRY_H_