// Copyright 2026 Student
#include "PaymentSystem.h"

#include <fstream>
#include <new>

#include "PaymentExceptions.h"

using std::cout;
using std::string;
using std::to_string;

// ======================================================
// TRANSACTION
// ======================================================
Transaction::Transaction()
    : id(0),
    date("Не вказано"),
    amount(0),
    type("Невідомо"),
    currency("UAH"),
    commission(0),
    status("Створено"),
    description("Немає опису") {
}

Transaction::Transaction(int id, string date, double amount, string type,
    string currency, double commission, string status,
    string description)
    : id(id),
    date(date),
    amount(amount),
    type(type),
    currency(currency),
    commission(commission),
    status(status),
    description(description) {
}

Transaction::Transaction(const Transaction& other)
    : id(other.id),
    date(other.date),
    amount(other.amount),
    type(other.type),
    currency(other.currency),
    commission(other.commission),
    status(other.status),
    description(other.description) {
}

Transaction::~Transaction() {}

void Transaction::display() const {
    cout << "ID: " << id << " | Дата: " << date << " | Сума: " << amount << " "
        << currency << " | Тип: " << type << " | Комісія: " << commission
        << " | Статус: " << status << "\n";
}

double Transaction::getAmount() const { return amount; }
string Transaction::getType() const { return type; }

// ======================================================
// CREDITCARD
// ======================================================
CreditCard::CreditCard()
    : cardNumber("0000"),
    holder("Невідомий"),
    expiryDate("00/00"),
    cardType("Debit"),
    creditLimit(0),
    usedCredit(0),
    blocked(false),
    bankName("Банк") {
}

CreditCard::CreditCard(string number, string holder, string expiry, string type,
    double limit, double used, bool blocked, string bank)
    : cardNumber(number),
    holder(holder),
    expiryDate(expiry),
    cardType(type),
    creditLimit(limit),
    usedCredit(used),
    blocked(blocked),
    bankName(bank) {
}

CreditCard::CreditCard(const CreditCard& other)
    : cardNumber(other.cardNumber),
    holder(other.holder),
    expiryDate(other.expiryDate),
    cardType(other.cardType),
    creditLimit(other.creditLimit),
    usedCredit(other.usedCredit),
    blocked(other.blocked),
    bankName(other.bankName) {
}

CreditCard::~CreditCard() {}

void CreditCard::display() const {
    cout << "Картка: " << cardNumber << " | Власник: " << holder
        << " | Ліміт: " << creditLimit
        << " | Статус: " << (blocked ? "Заблокована" : "Активна") << "\n";
}

void CreditCard::block() { blocked = true; }
void CreditCard::unblock() { blocked = false; }
bool CreditCard::isBlocked() const { return blocked; }
double CreditCard::getCreditLimit() const { return creditLimit; }
double CreditCard::getUsedCredit() const { return usedCredit; }
string CreditCard::getCardNumber() const { return cardNumber; }
void CreditCard::setCreditLimit(double limit) {
    if (limit >= 0) creditLimit = limit;
}

// ======================================================
// BANKACCOUNT ТА ПЕРЕВАНТАЖЕНІ ОПЕРАТОРИ
// ======================================================
BankAccount::BankAccount()
    : PaymentEntity("Невідомий", "UAH"),
    accountNumber("000000"),
    owner("Невідомий"),
    balance(0),
    active(true),
    bankName("Банк"),
    openedDate("Не вказано"),
    transactionCount(0) {
}

BankAccount::BankAccount(string number, string owner, double balance,
    string currency, bool active, string bank, string date,
    int count)
    : PaymentEntity(owner, currency),
    accountNumber(number),
    owner(owner),
    balance(balance),
    active(active),
    bankName(bank),
    openedDate(date),
    transactionCount(count) {
}

BankAccount::BankAccount(const BankAccount& other)
    : PaymentEntity(other),
    accountNumber(other.accountNumber),
    owner(other.owner),
    balance(other.balance),
    active(other.active),
    bankName(other.bankName),
    openedDate(other.openedDate),
    transactionCount(other.transactionCount) {
}

BankAccount::~BankAccount() {}

// ПУНКТ 2: Бінарний оператор + (повертає копію рахунку з поповненим балансом)
BankAccount BankAccount::operator+(double amount) const {
    BankAccount result(*this);
    if (amount > 0 && result.active) {
        result.balance += amount;
        ++result.transactionCount;
    }
    return result;
}

// ПУНКТ 2: Бінарний оператор - (повертає копію рахунку зі зменшеним балансом)
BankAccount BankAccount::operator-(double amount) const {
    BankAccount result(*this);
    if (amount > 0 && result.active && result.balance >= amount) {
        result.balance -= amount;
        ++result.transactionCount;
    }
    return result;
}

// ПУНКТ 3: Оператор * (розрахунок балансу з нарахуванням відсоткового коефіцієнта)
BankAccount BankAccount::operator*(double multiplier) const {
    BankAccount result(*this);
    if (multiplier >= 0 && result.active) {
        result.balance *= multiplier;
    }
    return result;
}

// ПУНКТ 3: Оператор присвоєння =
BankAccount& BankAccount::operator=(const BankAccount& other) {
    if (this != &other) {
        entityOwner = other.entityOwner;
        currency = other.currency;
        accountNumber = other.accountNumber;
        owner = other.owner;
        balance = other.balance;
        active = other.active;
        bankName = other.bankName;
        openedDate = other.openedDate;
        transactionCount = other.transactionCount;
    }
    return *this;
}

// ПУНКТ 3: Оператор складеного додавання +=
BankAccount& BankAccount::operator+=(double amount) {
    if (amount > 0 && active) {
        balance += amount;
        ++transactionCount;
    }
    return *this;
}

// ПУНКТ 3: Оператор складеного віднімання -=
BankAccount& BankAccount::operator-=(double amount) {
    if (amount > 0 && active && balance >= amount) {
        balance -= amount;
        ++transactionCount;
    }
    return *this;
}

// ПУНКТ 3: Оператор складеного множення *=
BankAccount& BankAccount::operator*=(double multiplier) {
    if (multiplier >= 0 && active) {
        balance *= multiplier;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const BankAccount& acc) {
    os << "Рахунок №" << acc.accountNumber << " (" << acc.owner
        << ") | Баланс: " << acc.balance << " " << acc.currency;
    return os;
}

bool BankAccount::processTransaction(double amount) {
    double total = amount + calculateCommission(amount);
    if (active && amount > 0 && balance >= total) {
        balance -= total;
        ++transactionCount;
        cout << "[BankAccount] Проведено базовий платіж на " << amount << " "
            << currency << " (комісія " << calculateCommission(amount) << ").\n";
        return true;
    }
    return false;
}

double BankAccount::calculateCommission(double amount) const {
    return amount * 0.015;
}

void BankAccount::displayAccount() const {
    cout << "[BankAccount] №" << accountNumber << " | Власник: " << owner
        << " | Баланс: " << balance << " " << currency << " | Банк: " << bankName
        << " | Статус: " << (active ? "Активний" : "Анульований") << "\n";
}

double BankAccount::getBalance() const { return balance; }
string BankAccount::getAccountNumber() const { return accountNumber; }
bool BankAccount::isActive() const { return active; }

void BankAccount::deposit(double amount) {
    if (amount > 0 && active) {
        balance += amount;
        ++transactionCount;
    }
}

void BankAccount::withdraw(double amount) {
    if (amount > 0 && active && balance >= amount) {
        balance -= amount;
        ++transactionCount;
    }
}

void BankAccount::cancelAccount() { active = false; }
void BankAccount::display() const { displayAccount(); }

// ======================================================
// PAYMENTHISTORY ТА ОПЕРАТОР ІНДЕКСАЦІЇ []
// ======================================================
PaymentHistory::PaymentHistory()
    : PaymentEntity("Невідомий", "UAH"),
    historyOwner("Невідомий"),
    historyId(0),
    totalTransactions(0),
    totalAmount(0),
    creationDate("Не вказано"),
    lastOperation("Немає"),
    historyActive(true) {
}

PaymentHistory::PaymentHistory(string owner, int id, string date)
    : PaymentEntity(owner, "UAH"),
    historyOwner(owner),
    historyId(id),
    totalTransactions(0),
    totalAmount(0),
    creationDate(date),
    lastOperation("Створено"),
    historyActive(true) {
}

PaymentHistory::PaymentHistory(const PaymentHistory& other)
    : PaymentEntity(other),
    transactions(other.transactions),
    historyOwner(other.historyOwner),
    historyId(other.historyId),
    totalTransactions(other.totalTransactions),
    totalAmount(other.totalAmount),
    creationDate(other.creationDate),
    lastOperation(other.lastOperation),
    historyActive(other.historyActive) {
}

PaymentHistory::~PaymentHistory() {}

// ПУНКТ 3: Оператор індексації [] (неконстантна версія)
Transaction& PaymentHistory::operator[](size_t index) {
    if (index >= transactions.size()) {
        throw ArrayBoundsException("Індекс транзакції виходить за межі журналу!");
    }
    return transactions[index];
}

// ПУНКТ 3: Оператор індексації [] (константна версія)
const Transaction& PaymentHistory::operator[](size_t index) const {
    if (index >= transactions.size()) {
        throw ArrayBoundsException("Індекс транзакції виходить за межі журналу!");
    }
    return transactions[index];
}

bool PaymentHistory::processTransaction(double amount) {
    Transaction t(++totalTransactions, "19.11.2026", amount, "Запис у журнал",
        currency, 0.0, "Зафіксовано", "Автоматичний запис");
    addTransaction(t);
    return true;
}

double PaymentHistory::calculateCommission(double amount) const { return 0.0; }
void PaymentHistory::display() const { showHistory(); }

void PaymentHistory::addTransaction(const Transaction& transaction) {
    if (historyActive) {
        transactions.push_back(transaction);
        ++totalTransactions;
        totalAmount += transaction.getAmount();
        lastOperation = transaction.getType();
    }
}

void PaymentHistory::showHistory() const {
    cout << "[PaymentHistory] Власник: " << historyOwner
        << " | Транзакцій: " << totalTransactions
        << " | Загальна сума: " << totalAmount << "\n";
    for (const auto& t : transactions) t.display();
}

int PaymentHistory::getTransactionCount() const { return totalTransactions; }
double PaymentHistory::getTotalAmount() const { return totalAmount; }
void PaymentHistory::clearHistory() {
    transactions.clear();
    totalTransactions = 0;
    totalAmount = 0;
}

// ======================================================
// COMMISSIONPOLICY
// ======================================================
CommissionPolicy::CommissionPolicy()
    : PaymentEntity("Банк", "UAH"),
    transferCommission(0.02),
    paymentCommission(0.0),
    withdrawalCommission(0.01),
    policyName("Стандартна"),
    policyActive(true),
    minimumCommission(1.0),
    maximumCommission(500.0) {
}

CommissionPolicy::CommissionPolicy(double transfer, double payment,
    double withdrawal, string name,
    string currency, bool active, double minimum,
    double maximum)
    : PaymentEntity("Банк", currency),
    transferCommission(transfer),
    paymentCommission(payment),
    withdrawalCommission(withdrawal),
    policyName(name),
    policyActive(active),
    minimumCommission(minimum),
    maximumCommission(maximum) {
}

CommissionPolicy::CommissionPolicy(const CommissionPolicy& other)
    : PaymentEntity(other),
    transferCommission(other.transferCommission),
    paymentCommission(other.paymentCommission),
    withdrawalCommission(other.withdrawalCommission),
    policyName(other.policyName),
    policyActive(other.policyActive),
    minimumCommission(other.minimumCommission),
    maximumCommission(other.maximumCommission) {
}

CommissionPolicy::~CommissionPolicy() {}

bool CommissionPolicy::processTransaction(double amount) {
    return policyActive;
}

double CommissionPolicy::calculateCommission(double amount) const {
    return calculateCommissionByType(amount, "Переказ");
}

void CommissionPolicy::display() const { displayPolicy(); }

double CommissionPolicy::calculateCommissionByType(double amount,
    string type) const {
    if (!policyActive || amount <= 0) return 0;
    double rate =
        (type == "Переказ")
        ? transferCommission
        : ((type == "Зняття") ? withdrawalCommission : paymentCommission);
    double comm = amount * rate;
    if (comm > 0 && comm < minimumCommission) comm = minimumCommission;
    if (comm > maximumCommission) comm = maximumCommission;
    return comm;
}

void CommissionPolicy::displayPolicy() const {
    cout << "[CommissionPolicy] Політика: " << policyName
        << " | Переказ: " << transferCommission * 100 << "%\n";
}

void CommissionPolicy::setTransferCommission(double value) {
    if (value >= 0) transferCommission = value;
}

double CommissionPolicy::getTransferCommission() const {
    return transferCommission;
}
bool CommissionPolicy::isPolicyActive() const { return policyActive; }

// ======================================================
// CLIENT
// ======================================================
Client::Client()
    : PaymentEntity("Невідомий", "UAH"),
    BankAccount(),
    clientId(0),
    email("Немає"),
    phone("Немає"),
    address("Немає"),
    card(),
    requestedLimit(0),
    registrationDate("Не вказано") {
}

Client::Client(string number, string owner, double balance, string currency,
    bool active, string bank, string date, int count, int id,
    string email, string phone, string address, CreditCard card)
    : PaymentEntity(owner, currency),
    BankAccount(number, owner, balance, currency, active, bank, date, count),
    clientId(id),
    email(email),
    phone(phone),
    address(address),
    card(card),
    requestedLimit(0),
    registrationDate(date) {
}

Client::Client(const Client& other)
    : PaymentEntity(other),
    BankAccount(other),
    clientId(other.clientId),
    email(other.email),
    phone(other.phone),
    address(other.address),
    card(other.card),
    history(other.history),
    requestedLimit(other.requestedLimit),
    registrationDate(other.registrationDate) {
}

Client::~Client() {}

bool Client::processTransaction(double amount) {
    payOrder(amount);
    return true;
}

double Client::calculateCommission(double amount) const {
    return amount * 0.02;
}

void Client::display() const { showClientInfo(); }

void Client::payOrder(double amount) {
    if (!active || card.isBlocked() || amount <= 0) return;
    if (balance + card.getCreditLimit() >= amount) {
        balance -= amount;
        ++transactionCount;
        history.push_back(Transaction(transactionCount, "19.11.2026", amount,
            "Оплата", currency, 0, "Успішно",
            "Оплата замовлення"));
    }
}

void Client::transfer(double amount, string targetAccount) {
    if (!active || card.isBlocked() || amount <= 0) return;
    double commission = calculateCommission(amount);
    double total = amount + commission;
    if (balance >= total) {
        balance -= total;
        ++transactionCount;
        history.push_back(Transaction(transactionCount, "19.11.2026", amount,
            "Переказ", currency, commission, "Успішно",
            "Переказ на " + targetAccount));
    }
}

void Client::blockCard() { card.block(); }
void Client::cancelClientAccount() { cancelAccount(); }
void Client::requestCreditLimit(double newLimit) {
    if (newLimit > 0) requestedLimit = newLimit;
}

void Client::showClientInfo() const {
    cout << "[Client #" << clientId << "] " << owner << " | Email: " << email
        << " | Рахунок: " << accountNumber << " | Баланс: " << balance << " "
        << currency << "\n";
    card.display();
}

void Client::showClientHistory() const {
    for (const auto& t : history) t.display();
}

void Client::addTransaction(const Transaction& transaction) {
    history.push_back(transaction);
}
CreditCard& Client::getCard() { return card; }
double Client::getRequestedLimit() const { return requestedLimit; }
void Client::approveCreditLimit(double limit) {
    if (limit > 0) {
        card.setCreditLimit(limit);
        requestedLimit = 0;
    }
}
void Client::rejectCreditLimit() { requestedLimit = 0; }

void Client::Write(const string& filename) const {
    if (filename.empty()) {
        throw EmptyFileNameException("Ім'я файлу для запису не може бути порожнім!");
    }
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw FileOpenException("Не вдалося відкрити файл для запису: " + filename);
    }
    out << accountNumber << "\n"
        << owner << "\n"
        << balance << "\n"
        << currency << "\n"
        << clientId << "\n"
        << email << "\n";
    if (out.fail()) {
        throw FileWriteException("Помилка запису у потік файлу: " + filename);
    }
    out.close();
}

void Client::Read(const string& filename) {
    if (filename.empty()) {
        throw EmptyFileNameException("Ім'я файлу для читання не може бути порожнім!");
    }
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw FileOpenException("Файл не знайдено або заблоковано: " + filename);
    }
    if (in.peek() == std::ifstream::traits_type::eof()) {
        throw FileReadException("Файл порожній: " + filename);
    }
    string acc, own, balStr, curr, idStr, mail;
    if (!std::getline(in, acc) || !std::getline(in, own) ||
        !std::getline(in, balStr) || !std::getline(in, curr) ||
        !std::getline(in, idStr) || !std::getline(in, mail)) {
        throw FileFormatException("Невірний формат даних у файлі: " + filename);
    }
    try {
        accountNumber = acc;
        owner = own;
        entityOwner = own;
        balance = std::stod(balStr);
        currency = curr;
        clientId = std::stoi(idStr);
        email = mail;
    }
    catch (...) {
        throw FileFormatException("Пошкоджені числові поля у файлі: " + filename);
    }
    in.close();
}

// ======================================================
// PREMIUMCLIENT
// ======================================================
PremiumClient::PremiumClient()
    : PaymentEntity("Преміум Клієнт", "UAH"),
    Client(),
    CommissionPolicy(),
    PaymentHistory(),
    premiumId(1),
    membershipLevel("Gold"),
    bonusBalance(500.0),
    bonusPoints(100),
    membershipDate("19.11.2026"),
    premiumActive(true),
    cashbackRate(0.05),
    privileges("Кешбек 5% + пільгова комісія") {
}

PremiumClient::PremiumClient(string number, string owner, double balance,
    string currency, CreditCard card, string level,
    double cashback)
    : PaymentEntity(owner, currency),
    Client(number, owner, balance, currency, true, "ПриватБанк", "19.11.2026",
        0, 100, "vip@bank.ua", "+38000000000", "Київ", card),
    CommissionPolicy(0.005, 0.0, 0.005, "VIP Тариф", currency, true, 0.0,
        100.0),
    PaymentHistory(owner, 100, "19.11.2026"),
    premiumId(100),
    membershipLevel(level),
    bonusBalance(0.0),
    bonusPoints(0),
    membershipDate("19.11.2026"),
    premiumActive(true),
    cashbackRate(cashback),
    privileges("Персональний менеджер") {
}

PremiumClient::PremiumClient(const PremiumClient& other)
    : PaymentEntity(other),
    Client(other),
    CommissionPolicy(other),
    PaymentHistory(other),
    premiumId(other.premiumId),
    membershipLevel(other.membershipLevel),
    bonusBalance(other.bonusBalance),
    bonusPoints(other.bonusPoints),
    membershipDate(other.membershipDate),
    premiumActive(other.premiumActive),
    cashbackRate(other.cashbackRate),
    privileges(other.privileges) {
}

PremiumClient::~PremiumClient() {}

bool PremiumClient::processTransaction(double amount) {
    bonusBalance += amount * cashbackRate;
    return true;
}

double PremiumClient::calculateCommission(double amount) const {
    return amount * 0.005;
}

void PremiumClient::display() const { displayPremiumInfo(); }

void PremiumClient::displayPremiumInfo() const {
    cout << "[PremiumClient] Власник: " << owner
        << " | Рівень: " << membershipLevel
        << " | Бонуси: " << bonusBalance << "\n";
}

void PremiumClient::makePremiumTransfer(double amount, string target) {
    double comm = calculateCommission(amount);
    Client::transfer(amount + comm, target);
}

void PremiumClient::showPremiumHistory() const { PaymentHistory::showHistory(); }

// ======================================================
// СЕРВІСНІ ФУНКЦІЇ
// ======================================================
void Service(Client& client, const string& filename) {
    client.Write(filename);
    client.Read(filename);
}

void Service_Static(BankAccount arr[], int& size, double minBalance) {
    if (size <= 0) throw EmptyContainerException("Масив порожній!");
    int newSize = 0;
    for (int i = 0; i < size; ++i) {
        if (arr[i].getBalance() >= minBalance) arr[newSize++] = arr[i];
    }
    size = newSize;
}

void Service_Dinamic(BankAccount*& arr, int& size, double minBalance) {
    if (arr == nullptr || size <= 0)
        throw EmptyContainerException("Масив не існує!");
    int keepCount = 0;
    for (int i = 0; i < size; ++i) {
        if (arr[i].getBalance() >= minBalance) ++keepCount;
    }
    BankAccount* newArr = new (std::nothrow) BankAccount[keepCount];
    if (newArr == nullptr)
        throw MemoryAllocationException("Помилка виділення пам'яті!");
    int idx = 0;
    for (int i = 0; i < size; ++i) {
        if (arr[i].getBalance() >= minBalance) newArr[idx++] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    size = keepCount;
}