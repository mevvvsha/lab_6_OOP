#pragma once
#ifndef PAYMENT_EXCEPTIONS_H_
#define PAYMENT_EXCEPTIONS_H_

#include <exception>
#include <string>

// Базовий клас винятків для всієї платіжної системи
class PaymentSystemException : public std::exception {
public:
    explicit PaymentSystemException(const std::string& msg) : message_(msg) {}
    // Повертає текст повідомлення про помилку
    const char* what() const noexcept override { return message_.c_str(); }

protected:
    std::string message_;
};

// --- Винятки при роботі з динамічною пам'яттю та масивами ---
class MemoryAllocationException : public PaymentSystemException {
public:
    explicit MemoryAllocationException(const std::string& msg)
        : PaymentSystemException("Помилка динамічної пам'яті: " + msg) {
    }
};

class ArrayBoundsException : public PaymentSystemException {
public:
    explicit ArrayBoundsException(const std::string& msg)
        : PaymentSystemException("Помилка індексації або розміру масиву: " + msg) {
    }
};

class EmptyContainerException : public PaymentSystemException {
public:
    explicit EmptyContainerException(const std::string& msg)
        : PaymentSystemException("Помилка видалення з порожнього контейнера: " + msg) {
    }
};

// --- Винятки при роботі з файлами (читання / запис) ---
class EmptyFileNameException : public PaymentSystemException {
public:
    explicit EmptyFileNameException(const std::string& msg)
        : PaymentSystemException("Помилка імені файлу: " + msg) {
    }
};

class FileOpenException : public PaymentSystemException {
public:
    explicit FileOpenException(const std::string& msg)
        : PaymentSystemException("Помилка відкриття файлу: " + msg) {
    }
};

class FileWriteException : public PaymentSystemException {
public:
    explicit FileWriteException(const std::string& msg)
        : PaymentSystemException("Помилка запису у файл: " + msg) {
    }
};

class FileReadException : public PaymentSystemException {
public:
    explicit FileReadException(const std::string& msg)
        : PaymentSystemException("Помилка читання з файлу: " + msg) {
    }
};

class FileFormatException : public PaymentSystemException {
public:
    explicit FileFormatException(const std::string& msg)
        : PaymentSystemException("Пошкоджений формат даних у файлі: " + msg) {
    }
};

#endif  // PAYMENT_EXCEPTIONS_H_
