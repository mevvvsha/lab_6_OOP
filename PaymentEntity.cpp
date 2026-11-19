#include "PaymentEntity.h"

// Ініціалізація статичного лічильника об'єктів
int PaymentEntity::objectCount = 0;

PaymentEntity::PaymentEntity() : entityOwner("Невідомий"), currency("UAH") {
    ++objectCount;  // Збільшуємо лічильник при створенні об'єкта
}

PaymentEntity::PaymentEntity(std::string owner, std::string currency)
    : entityOwner(owner), currency(currency) {
    ++objectCount;
}

PaymentEntity::PaymentEntity(const PaymentEntity& other)
    : entityOwner(other.entityOwner), currency(other.currency) {
    ++objectCount;
}

PaymentEntity::~PaymentEntity() {
    --objectCount;  // Зменшуємо лічильник при знищенні об'єкта
    std::cout << "[Віртуальний деструктор ~PaymentEntity] Знищено сутність: "
        << entityOwner << "\n";
}

void PaymentEntity::display() const {
    std::cout << "Сутність власника: " << entityOwner
        << " | Валюта: " << currency << "\n";
}

// Базовий розрахунок комісії (1%)
double PaymentEntity::calculateCommission(double amount) const {
    return amount * 0.01;
}

std::string PaymentEntity::getEntityOwner() const { return entityOwner; }

void PaymentEntity::setEntityOwner(const std::string& owner) {
    entityOwner = owner;
}

// Статичний метод виводу кількості живих об'єктів
void PaymentEntity::showCreatedObjectsCount() {
    std::cout << "[Статичний метод PaymentEntity] Активних об'єктів у системі: "
        << objectCount << "\n";
}

int PaymentEntity::getObjectCount() { return objectCount; }