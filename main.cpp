#include <windows.h>
#include <iostream>
#include <string>
#include "FinancialRegistry.h"
#include "PaymentEntity.h"
#include "PaymentExceptions.h"
#include "PaymentSystem.h"

using std::cerr;
using std::cout;
using std::string;

int main() {
    // Налаштування UTF-8 у консолі для коректного відображення українських літер
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "=========================================================\n";
    cout << "   ЛАБОРАТОРНА РОБОТА №6: ПЕРЕВАНТАЖЕННЯ ОПЕРАТОРІВ\n";
    cout << "   ТА ШАБЛОНИ КЛАСІВ (Варіант 14: Система Платежі)\n";
    cout << "=========================================================\n\n";

    // =========================================================
    // ПУНКТ 2: Демонстрація бінарних операторів + та -
    // =========================================================
    cout << ">>> ПУНКТ 2: Бінарні оператори '+' та '-' (BankAccount) <<<\n";
    BankAccount acc1("UA1001", "Іван Петренко", 10000.0, "UAH", true,
        "ПриватБанк", "01.10.2026", 0);
    cout << "Початковий рахунок: " << acc1 << "\n";

    BankAccount accAfterDeposit = acc1 + 2500.0;
    cout << "Після (acc1 + 2500): " << accAfterDeposit << "\n";

    BankAccount accAfterWithdraw = accAfterDeposit - 4000.0;
    cout << "Після (accAfterDeposit - 4000): " << accAfterWithdraw << "\n\n";

    // =========================================================
    // ПУНКТ 3: Демонстрація операторів *, =, +=, -=, *= та []
    // =========================================================
    cout << ">>> ПУНКТ 3: Оператори '*', '=', '+=', '-=', '*=' <<<\n";
    BankAccount accBonus = acc1 * 1.05;  // Нарахування +5% річних
    cout << "Множення на коефіцієнт (acc1 * 1.05): " << accBonus << "\n";

    BankAccount accCopy;
    accCopy = acc1;  // Оператор присвоєння =
    cout << "Копія через присвоєння (accCopy = acc1): " << accCopy << "\n";

    accCopy += 1500.0;  // Складене додавання +=
    cout << "Після (accCopy += 1500): " << accCopy << "\n";

    accCopy -= 500.0;   // Складене віднімання -=
    cout << "Після (accCopy -= 500): " << accCopy << "\n";

    accCopy *= 1.10;    // Складене множення *= (+10%)
    cout << "Після (accCopy *= 1.10): " << accCopy << "\n\n";

    cout << ">>> ПУНКТ 3: Оператор індексації '[]' (PaymentHistory) <<<\n";
    PaymentHistory history("Іван Петренко", 101, "19.11.2026");
    history.addTransaction(Transaction(1, "18.11.2026", 1200.0, "Оплата",
        "UAH", 0.0, "Успішно", "Супермаркет"));
    history.addTransaction(Transaction(2, "19.11.2026", 3500.0, "Переказ",
        "UAH", 35.0, "Успішно", "Оренда"));

    try {
        cout << "Доступ до транзакції history[0]: ";
        history[0].display();

        cout << "Доступ до транзакції history[1]: ";
        history[1].display();

        // Перевірка виходу за межі індексу
        cout << "Спроба доступу до неіснуючого індексу history[5]: ";
        history[5].display();
    }
    catch (const PaymentSystemException& ex) {
        cerr << "[Перехоплено виняток індексації] " << ex.what() << "\n\n";
    }

    // =========================================================
    // ПУНКТ 5: Шаблон класу FinancialRegistry<T>
    // (5 шаблонних полів даних + шаблонні методи)
    // =========================================================
    cout << ">>> ПУНКТ 5: Шаблон класу FinancialRegistry<T> <<<\n";

    // 1. Інстанціювання шаблону для типу double (облік платежів)
    FinancialRegistry<double> paymentAmountsRegistry(
        "Реєстр сум платежів (UAH)", 5000.0, 1000.0, 100.0, 50000.0);
    paymentAmountsRegistry.addItem(1250.50);
    paymentAmountsRegistry.addItem(3400.00);
    paymentAmountsRegistry.addItem(890.25);

    paymentAmountsRegistry.displayRegistry();
    cout << "Загальна сума елементів у реєстрі (calculateTotalSum): "
        << paymentAmountsRegistry.calculateTotalSum() << " UAH\n";

    double prevPrimary = paymentAmountsRegistry.swapPrimaryItem(7500.0);
    cout << "Замінено основний ліміт (попередній: " << prevPrimary
        << ", новий: 7500)\n\n";

    // 2. Інстанціювання шаблону для типу std::string (коди валют / IBAN)
    FinancialRegistry<string> currencyRegistry(
        "Реєстр підтримуваних валют", "UAH", "USD", "EUR", "GBP");
    currencyRegistry.addItem("UAH - Українська гривня");
    currencyRegistry.addItem("USD - Долар США");
    currencyRegistry.addItem("EUR - Євро");
    currencyRegistry.displayRegistry();
    cout << "Отримано елемент за індексом 1: " << currencyRegistry.getItem(1)
        << "\n\n";

    // 3. Інстанціювання шаблону для власного класу BankAccount
    BankAccount vip1("UA9001", "Олена Коваль", 45000.0, "UAH", true,
        "Монобанк", "10.10.2026", 2);
    BankAccount vip2("UA9002", "Тарас Шевченко", 82000.0, "UAH", true,
        "Ощадбанк", "12.10.2026", 5);
    FinancialRegistry<BankAccount> vipAccountsRegistry(
        "VIP-рахунки відділення", vip1, vip2, acc1, accBonus);
    vipAccountsRegistry.addItem(vip1);
    vipAccountsRegistry.addItem(vip2);
    vipAccountsRegistry.displayRegistry();

    cout << "\nРоботу програми успішно завершено.\n";
    return 0;
}