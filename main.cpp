#include "store.h"

bool checkIfDataExists() {
    ifstream productFile("products.bin", ios::binary);
    ifstream sellerFile("sellers.bin", ios::binary);
    ifstream buyerFile("buyers.bin", ios::binary);

    bool exists = productFile.is_open() || sellerFile.is_open() || buyerFile.is_open();

    productFile.close();
    sellerFile.close();
    buyerFile.close();

    return exists;
}

int main() {
    srand(time(nullptr));

    cout << "=== STORE MANAGEMENT SYSTEM ===" << endl;
    cout << "With Binary File Storage and Interactive Purchases" << endl;

    // Створення менеджера та магазину
    Manager manager("John Doe", "john.doe@store.com");
    Store store(manager, 15000.0);

    // Перевіряємо, чи є збережені дані
    bool hasExistingData = checkIfDataExists();

    if (hasExistingData) {
        cout << "\nFound existing data in binary files." << endl;
        char useExisting;
        cout << "Do you want to use existing data? (y/n): ";
        useExisting = getCharInput("");

        if (useExisting == 'n' || useExisting == 'N') {
            store.clearData();
            cout << "Existing data cleared." << endl;
            hasExistingData = false;
        } else {
            // Використовуємо існуючі дані, вмикаємо автозбереження
            store.setAutoSave(true);
            cout << "Using existing data with auto-save enabled." << endl;
        }
    }

    if (!hasExistingData) {
        // Основний вибір: автоматичне чи ручне заповнення
        cout << "\nChoose data input method:" << endl;
        cout << "A - Automatic (sample data, not saved to files)" << endl;
        cout << "M - Manual (enter data yourself, saved to files)" << endl;

        char choice;
        do {
            choice = getCharInput("Enter your choice (A/M): ");
            choice = toupper(choice);
        } while (choice != 'A' && choice != 'M');

        if (choice == 'A') {
            // Автоматичне заповнення даними без збереження
            store.setAutoSave(false);
            initializeStoreWithSampleData(store);
            cout << "Sample data loaded (not saved to files)." << endl;
        } else {
            // Ручне введення даних з автозбереженням
            store.setAutoSave(true);
            manualDataInput(store);
            cout << "Manual data input completed and saved to files." << endl;
        }
    }

    this_thread::sleep_for(chrono::seconds(2));

    // Показ початкового стану
    cout << "\n\n=== INITIAL STORE STATE ===" << endl;
    store.displayStatus();
    store.displayProducts();
    store.displaySellers();
    store.displayBuyers();

    this_thread::sleep_for(chrono::seconds(3));

    // Запит чи хоче користувач ввести покупки вручну
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

    // Запит кількості днів для симуляції
    int days = getIntInput("\nEnter number of days to simulate: ");

    // Симуляція днів роботи
    for (int day = 1; day <= days; day++) {
        simulateDay(store, day, !store.getAutoSave());
        this_thread::sleep_for(chrono::seconds(2));
    }

    // Фінальний звіт
    cout << "\n\n=== FINAL RESULTS ===" << endl;
    store.displayStatus();
    store.displayFinancialReport();

    cout << "\n=== SIMULATION COMPLETED ===" << endl;
    cout << "Simulated " << days << " days of store operation." << endl;
    cout << "Data storage: " << (store.getAutoSave() ? "Enabled" : "Disabled") << endl;
    cout << "Final balance: " << fixed << setprecision(2)
          << store.getBalance() << " USD" << endl;

    if (store.getAutoSave()) {
        cout << "All data has been saved to binary files!" << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}