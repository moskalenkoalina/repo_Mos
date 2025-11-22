#include "store.h"

int main() {
    srand(time(nullptr));

    cout << "=== STORE MANAGEMENT SYSTEM ===" << endl;
    cout << "With Interactive Purchases" << endl;

    // Створення менеджера та магазину
    Manager manager("John Doe", "john.doe@store.com");
    Store store(manager, 15000.0);

    // Основний вибір: автоматичне чи ручне заповнення
    cout << "\nChoose data input method:" << endl;
    cout << "A - Automatic (sample data with random purchases)" << endl;
    cout << "M - Manual (enter data yourself with manual purchases)" << endl;

    char choice;
    do {
        choice = getCharInput("Enter your choice (A/M): ");
        choice = toupper(choice);
    } while (choice != 'A' && choice != 'M');

    bool automaticMode = (choice == 'A');

    if (automaticMode) {
        // Автоматичне заповнення даними
        initializeStoreWithSampleData(store);
    } else {
        // Ручне введення даних
        manualDataInput(store);
    }

    this_thread::sleep_for(chrono::seconds(2));

    // Показ початкового стану
    cout << "\n\n=== INITIAL STORE STATE ===" << endl;
    store.displayStatus();
    store.displayProducts();
    store.displaySellers();
    store.displayBuyers();

    this_thread::sleep_for(chrono::seconds(3));

    if (!automaticMode) {
        // У ручному режимі пропонуємо ввести покупки
        char purchaseChoice;
        cout << "\nDo you want to enter purchases manually? (y/n): ";
        purchaseChoice = getCharInput("");

        if (purchaseChoice == 'y' || purchaseChoice == 'Y') {
            manualPurchasesInput(store);

            // Показ стану після ручних покупок
            cout << "\n=== STORE STATE AFTER MANUAL PURCHASES ===" << endl;
            store.displayStatus();
            store.displayFinancialReport();
            this_thread::sleep_for(chrono::seconds(3));
        }
    }

    // Запит кількості днів для симуляції
    int days = getIntInput("\nEnter number of days to simulate: ");

    // Симуляція днів роботи
    for (int day = 1; day <= days; day++) {
        simulateDay(store, day, automaticMode);
        this_thread::sleep_for(chrono::seconds(2));
    }

    // Фінальний звіт
    cout << "\n\n=== FINAL RESULTS ===" << endl;
    store.displayStatus();
    store.displayFinancialReport();

    cout << "\n=== SIMULATION COMPLETED ===" << endl;
    cout << "Simulated " << days << " days of store operation." << endl;
    cout << "Mode: " << (automaticMode ? "Automatic" : "Manual") << endl;
    cout << "Final balance: " << fixed << setprecision(2)
          << store.getBalance() << " USD" << endl;

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}