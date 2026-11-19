#pragma once
#ifndef PAYMENT_ENTITY_H_
#define PAYMENT_ENTITY_H_

#include <iostream>
#include <string>

// Базовий абстрактний клас для всієї ієрархії платіжної системи
class PaymentEntity {
protected:
    std::string entityOwner;  // Власник рахунку чи картки
    std::string currency;     // Валюта

private:
    static int objectCount;   // Спільний лічильник створених об'єктів для всіх нащадків

public:
    PaymentEntity();
    PaymentEntity(std::string owner, std::string currency);
    PaymentEntity(const PaymentEntity& other);

    // Віртуальний деструктор: гарантує правильне видалення похідних об'єктів через покажчик Base*
    virtual ~PaymentEntity();

    // Чисто віртуальна функція (= 0): робить клас абстрактним, вимагає реалізації у нащадках
    virtual bool processTransaction(double amount) = 0;

    // Звичайні віртуальні функції (реалізують поліморфізм / пізнє зв'язування)
    virtual void display() const;
    virtual double calculateCommission(double amount) const;

    std::string getEntityOwner() const;
    void setEntityOwner(const std::string& owner);

    // Статичні методи: викликаються через ім'я класу PaymentEntity::...
    static void showCreatedObjectsCount();
    static int getObjectCount();
};

// Допоміжні класи для демонстрації різниці між невіртуальним та віртуальним деструкторами
class NonVirtualBase {
public:
    NonVirtualBase() {}
    // Невіртуальний деструктор: при delete викличеться лише він
    ~NonVirtualBase() {
        std::cout << "[NonVirtualBase] Викликано ТІЛЬКИ базовий деструктор!\n";
    }
};

class NonVirtualDerived : public NonVirtualBase {
private:
    int* dynamicData;

public:
    NonVirtualDerived() { dynamicData = new int[5]; }
    ~NonVirtualDerived() {
        delete[] dynamicData;
        std::cout << "[NonVirtualDerived] Цей деструктор НЕ викликається при delete basePtr!\n";
    }
};

#endif  // PAYMENT_ENTITY_H_