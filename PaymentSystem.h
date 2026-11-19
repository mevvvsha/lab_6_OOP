#pragma once
#ifndef PAYMENT_SYSTEM_H_
#define PAYMENT_SYSTEM_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "PaymentEntity.h"

// Клас транзакції
class Transaction {
private:
    int id;
    std::string date;
    double amount;
    std::string type;
    std::string currency;
    double commission;
    std::string status;
    std::string description;

public:
    Transaction();
    Transaction(int id, std::string date, double amount, std::string type,
        std::string currency, double commission, std::string status,
        std::string description);
    Transaction(const Transaction& other);
    ~Transaction();

    void display() const;
    double getAmount() const;
    std::string getType() const;
};

// Клас кредитної картки
class CreditCard {
private:
    std::string cardNumber;
    std::string holder;
    std::string expiryDate;
    std::string cardType;
    double creditLimit;
    double usedCredit;
    bool blocked;
    std::string bankName;

public:
    CreditCard();
    CreditCard(std::string number, std::string holder, std::string expiry,
        std::string type, double limit, double used, bool blocked,
        std::string bank);
    CreditCard(const CreditCard& other);
    ~CreditCard();

    void display() const;
    void block();
    void unblock();
    bool isBlocked() const;
    double getCreditLimit() const;
    double getUsedCredit() const;
    std::string getCardNumber() const;
    void setCreditLimit(double limit);
};

// ======================================================
// БАНКІВСЬКИЙ РАХУНОК (Перевантаження +, -, *, =, +=, -=, *=)
// ======================================================
class BankAccount : virtual public PaymentEntity {
protected:
    std::string accountNumber;
    std::string owner;
    double balance;
    bool active;
    std::string bankName;
    std::string openedDate;
    int transactionCount;

public:
    BankAccount();
    BankAccount(std::string number, std::string owner, double balance,
        std::string currency, bool active, std::string bank,
        std::string date, int count);
    BankAccount(const BankAccount& other);
    ~BankAccount() override;

    // ПУНКТ 2: Перевантаження бінарних операторів + та - (функції-члени класу)
    BankAccount operator+(double amount) const;
    BankAccount operator-(double amount) const;

    // ПУНКТ 3: Перевантаження операторів *, =, +=, -=, *= (функції-члени класу)
    BankAccount operator*(double multiplier) const;
    BankAccount& operator=(const BankAccount& other);
    BankAccount& operator+=(double amount);
    BankAccount& operator-=(double amount);
    BankAccount& operator*=(double multiplier);

    // Дружній оператор виводу (для зручного використання у шаблонному класі)
    friend std::ostream& operator<<(std::ostream& os, const BankAccount& acc);

    bool processTransaction(double amount) override;
    void display() const override;
    double calculateCommission(double amount) const override;

    void displayAccount() const;
    double getBalance() const;
    std::string getAccountNumber() const;
    bool isActive() const;
    void deposit(double amount);
    void withdraw(double amount);
    void cancelAccount();
};

// ======================================================
// ІСТОРІЯ ПЛАТЕЖІВ (Перевантаження оператора індексації [])
// ======================================================
class PaymentHistory : virtual public PaymentEntity {
protected:
    std::vector<Transaction> transactions;
    std::string historyOwner;
    int historyId;
    int totalTransactions;
    double totalAmount;
    std::string creationDate;
    std::string lastOperation;
    bool historyActive;

public:
    PaymentHistory();
    PaymentHistory(std::string owner, int id, std::string date);
    PaymentHistory(const PaymentHistory& other);
    ~PaymentHistory() override;

    // ПУНКТ 3: Перевантаження оператора індексації [] (для читання та зміни)
    Transaction& operator[](size_t index);
    const Transaction& operator[](size_t index) const;

    bool processTransaction(double amount) override;
    void display() const override;
    double calculateCommission(double amount) const override;

    void addTransaction(const Transaction& transaction);
    void showHistory() const;
    int getTransactionCount() const;
    double getTotalAmount() const;
    void clearHistory();
};

// Політика комісій
class CommissionPolicy : virtual public PaymentEntity {
protected:
    double transferCommission;
    double paymentCommission;
    double withdrawalCommission;
    std::string policyName;
    bool policyActive;
    double minimumCommission;
    double maximumCommission;

public:
    CommissionPolicy();
    CommissionPolicy(double transfer, double payment, double withdrawal,
        std::string name, std::string currency, bool active,
        double minimum, double maximum);
    CommissionPolicy(const CommissionPolicy& other);
    ~CommissionPolicy() override;

    bool processTransaction(double amount) override;
    void display() const override;
    double calculateCommission(double amount) const override;

    double calculateCommissionByType(double amount, std::string type) const;
    void displayPolicy() const;
    void setTransferCommission(double value);
    double getTransferCommission() const;
    bool isPolicyActive() const;
};

// Клієнт
class Client : public BankAccount {
private:
    int clientId;
    std::string email;
    std::string phone;
    std::string address;
    CreditCard card;
    std::vector<Transaction> history;
    double requestedLimit;
    std::string registrationDate;

public:
    using BankAccount::getAccountNumber;
    using BankAccount::getBalance;

    Client();
    Client(std::string number, std::string owner, double balance,
        std::string currency, bool active, std::string bank, std::string date,
        int count, int id, std::string email, std::string phone,
        std::string address, CreditCard card);
    Client(const Client& other);
    ~Client() override;

    bool processTransaction(double amount) override;
    void display() const override;
    double calculateCommission(double amount) const override;

    void payOrder(double amount);
    void transfer(double amount, std::string targetAccount);
    void blockCard();
    void cancelClientAccount();
    void requestCreditLimit(double newLimit);
    void showClientInfo() const;
    void showClientHistory() const;
    void addTransaction(const Transaction& transaction);

    CreditCard& getCard();
    double getRequestedLimit() const;
    void approveCreditLimit(double limit);
    void rejectCreditLimit();

    void Write(const std::string& filename) const;
    void Read(const std::string& filename);
};

// Преміум-клієнт
class PremiumClient : public Client,
    public CommissionPolicy,
    public PaymentHistory {
private:
    int premiumId;
    std::string membershipLevel;
    double bonusBalance;
    int bonusPoints;
    std::string membershipDate;
    bool premiumActive;
    double cashbackRate;
    std::string privileges;

public:
    PremiumClient();
    PremiumClient(std::string number, std::string owner, double balance,
        std::string currency, CreditCard card, std::string level,
        double cashback);
    PremiumClient(const PremiumClient& other);
    ~PremiumClient() override;

    bool processTransaction(double amount) override;
    void display() const override;
    double calculateCommission(double amount) const override;

    void displayPremiumInfo() const;
    void makePremiumTransfer(double amount, std::string target);
    void showPremiumHistory() const;
};

// Сервісні функції
void Service(Client& client, const std::string& filename);
void Service_Static(BankAccount arr[], int& size, double minBalance);
void Service_Dinamic(BankAccount*& arr, int& size, double minBalance);

#endif  // PAYMENT_SYSTEM_H_
