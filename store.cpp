#include "store.h"

// Реалізація методів класу Person
Person::Person(const string& n, const string& c) : name(n), contact(c) {}

void Person::display() const {
    cout << "Name: " << name << ", Contact: " << contact;
}

string Person::getName() const { return name; }
string Person::getContact() const { return contact; }

// Реалізація методів класу Buyer
Buyer::Buyer(const string& n, const string& c, double disc, bool regular)
    : Person(n, c), discount(disc), purchasesCount(0), isRegular(regular), totalSpent(0) {}

double Buyer::getDiscount() const { return discount; }
void Buyer::incrementPurchases() { purchasesCount++; }
int Buyer::getPurchasesCount() const { return purchasesCount; }
bool Buyer::getIsRegular() const { return isRegular; }
void Buyer::setIsRegular(bool regular) { isRegular = regular; }
void Buyer::addToTotalSpent(double amount) { totalSpent += amount; }
double Buyer::getTotalSpent() const { return totalSpent; }

void Buyer::display() const {
    cout << "Buyer - ";
    Person::display();
    cout << ", Discount: " << fixed << setprecision(1) << (discount * 100) << "%, "
         << "Purchases: " << purchasesCount << ", Regular: " << (isRegular ? "Yes" : "No")
         << ", Total Spent: " << fixed << setprecision(2) << totalSpent << " USD" << endl;
}

// Реалізація методів класу Seller
Seller::Seller(const string& n, const string& c)
    : Person(n, c), salary(0), totalProfit(0), itemsSold(0) {}

void Seller::calculateSalary(double profitShare) {
    salary = totalProfit * profitShare;
}

void Seller::addSale(double itemProfit, int quantity) {
    totalProfit += itemProfit;
    itemsSold += quantity;
}

double Seller::getSalary() const { return salary; }
double Seller::getTotalProfit() const { return totalProfit; }
int Seller::getItemsSold() const { return itemsSold; }

void Seller::display() const {
    cout << "Seller - ";
    Person::display();
    cout << ", Salary: " << fixed << setprecision(2) << salary
         << " USD, Items Sold: " << itemsSold
         << ", Total Profit: " << totalProfit << " USD" << endl;
}

// Реалізація методів класу Manager
Manager::Manager(const string& n, const string& c) : Person(n, c) {}

void Manager::display() const {
    cout << "Manager - ";
    Person::display();
    cout << endl;
}

// Реалізація методів класу Product
Product::Product(const string& n, double p, double c, int q)
    : name(n), price(p), cost(c), quantity(q), soldQuantity(0) {}

double Product::getPrice() const { return price; }
double Product::getCost() const { return cost; }
int Product::getQuantity() const { return quantity; }
string Product::getName() const { return name; }

void Product::reduceQuantity(int count) {
    if (count > quantity) {
        throw invalid_argument("Not enough quantity of " + name);
    }
    quantity -= count;
}

void Product::addQuantity(int count) {
    quantity += count;
}

int Product::getSoldQuantity() const { return soldQuantity; }
void Product::addSoldQuantity(int q) { soldQuantity += q; }

double Product::getProfitPerItem() const {
    return price - cost; // Прибуток на одиницю товару
}

void Product::display() const {
    cout << "Product: " << name << ", Price: " << fixed << setprecision(2)
         << getPrice() << " USD, Cost: " << cost << " USD, Quantity: " << quantity
         << ", Sold: " << soldQuantity << ", Profit per item: " << getProfitPerItem() << " USD" << endl;
}

// Реалізація методів класу Store
Store::Store(const Manager& m, double initialBalance, const string& filename)
    : manager(m), balance(initialBalance), dataFile(filename) {}

Product* Store::findProduct(const string& productName) {
    for (auto& product : products) {
        if (product.getName() == productName) {
            return &product;
        }
    }
    return nullptr;
}

Seller* Store::findSeller(const string& sellerName) {
    for (auto& seller : sellers) {
        if (seller.getName() == sellerName) {
            return &seller;
        }
    }
    return nullptr;
}

Buyer* Store::findBuyer(const string& buyerName) {
    for (auto& buyer : buyers) {
        if (buyer.getName() == buyerName) {
            return &buyer;
        }
    }
    return nullptr;
}

void Store::addProduct(const Product& p) {
    products.push_back(p);
}

void Store::addSeller(const Seller& s) {
    sellers.push_back(s);
}

void Store::addBuyer(const Buyer& b) {
    buyers.push_back(b);
}

bool Store::purchaseItems(const string& buyerName, const string& sellerName,
                         const string& productName, int quantity) {
    try {
        Product* product = findProduct(productName);
        Seller* seller = findSeller(sellerName);
        Buyer* buyer = findBuyer(buyerName);

        if (!product) throw invalid_argument("Product not found: " + productName);
        if (!seller) throw invalid_argument("Seller not found: " + sellerName);
        if (!buyer) throw invalid_argument("Buyer not found: " + buyerName);
        if (quantity <= 0) throw invalid_argument("Quantity must be positive");

        if (product->getQuantity() < quantity) {
            throw invalid_argument("Not enough quantity of " + productName);
        }

        double basePrice = product->getPrice() * quantity;
        double finalPrice = basePrice * (1 - buyer->getDiscount());

        if (buyer->getIsRegular()) {
            finalPrice *= 0.9; // Додаткова знижка 10% для постійних клієнтів
        }

        // Розрахунок прибутку для продавця (різниця між ціною продажу та собівартістю)
        double profitPerItem = product->getProfitPerItem();
        double totalProfitForSeller = profitPerItem * quantity;

        // Оновлення даних
        product->reduceQuantity(quantity);
        product->addSoldQuantity(quantity);
        balance += finalPrice;

        // Продавець отримує прибуток від продажу
        seller->addSale(totalProfitForSeller, quantity);

        // Оновлення даних покупця
        buyer->incrementPurchases();
        buyer->addToTotalSpent(finalPrice);

        // Перевірка, чи покупець став постійним
        if (buyer->getPurchasesCount() >= 3 && !buyer->getIsRegular()) {
            buyer->setIsRegular(true);
            cout << "Buyer " << buyerName << " is now a regular customer!" << endl;
        }

        cout << "Purchase successful: " << quantity << " x " << productName
             << " for " << finalPrice << " USD (Profit: " << totalProfitForSeller << " USD)" << endl;

        return true;

    } catch (const exception& e) {
        cout << "Purchase failed: " << e.what() << endl;
        return false;
    }
}

void Store::restockProduct(const string& productName, int quantity) {
    Product* product = findProduct(productName);
    if (product) {
        product->addQuantity(quantity);
        cout << "Restocked " << quantity << " units of " << productName << endl;
    } else {
        cout << "Product not found for restocking: " << productName << endl;
    }
}

void Store::calculateSalaries(double profitShare) {
    double totalStoreProfit = 0;

    // Спочатку розраховуємо загальний прибуток магазину
    for (const auto& seller : sellers) {
        totalStoreProfit += seller.getTotalProfit();
    }

    // Тепер розраховуємо зарплату кожному продавцю
    for (auto& seller : sellers) {
        if (totalStoreProfit > 0) {
            double sellerShare = seller.getTotalProfit() / totalStoreProfit;
            seller.calculateSalary(profitShare * sellerShare);
        } else {
            seller.calculateSalary(0);
        }
    }

    cout << "Salaries calculated. Total store profit: " << totalStoreProfit
         << " USD, Profit share: " << (profitShare * 100) << "%" << endl;
}

void Store::simulateRandomPurchases(int numberOfTransactions) {
    random_device rd;
    mt19937 gen(rd());

    if (products.empty() || sellers.empty() || buyers.empty()) {
        cout << "Cannot simulate purchases: need products, sellers and buyers" << endl;
        return;
    }

    int successfulTransactions = 0;
    for (int i = 0; i < numberOfTransactions; ++i) {
        uniform_int_distribution<> productDist(0, products.size() - 1);
        uniform_int_distribution<> sellerDist(0, sellers.size() - 1);
        uniform_int_distribution<> buyerDist(0, buyers.size() - 1);
        uniform_int_distribution<> quantityDist(1, 5);

        int productIndex = productDist(gen);
        int sellerIndex = sellerDist(gen);
        int buyerIndex = buyerDist(gen);
        int quantity = quantityDist(gen);

        string productName = products[productIndex].getName();
        string sellerName = sellers[sellerIndex].getName();
        string buyerName = buyers[buyerIndex].getName();

        if (purchaseItems(buyerName, sellerName, productName, quantity)) {
            successfulTransactions++;
        }
    }

    cout << "Random purchases simulation: " << successfulTransactions
          << "/" << numberOfTransactions << " successful transactions" << endl;
}

void Store::simulateRandomRestocking() {
    random_device rd;
    mt19937 gen(rd());

    if (products.empty()) {
        cout << "No products to restock" << endl;
        return;
    }

    uniform_int_distribution<> productDist(0, products.size() - 1);
    uniform_int_distribution<> quantityDist(5, 20);

    int productIndex = productDist(gen);
    int quantity = quantityDist(gen);

    string productName = products[productIndex].getName();
    restockProduct(productName, quantity);
}

void Store::displayStatus() const {
    cout << "\n=== STORE STATUS ===" << endl;
    manager.display();
    cout << "Balance: " << fixed << setprecision(2) << balance << " USD" << endl;
    cout << "Products in stock: " << products.size() << endl;
    cout << "Sellers: " << sellers.size() << endl;
    cout << "Buyers: " << buyers.size() << endl;
}

void Store::displayProducts() const {
    cout << "\n=== PRODUCTS ===" << endl;
    if (products.empty()) {
        cout << "No products available." << endl;
    } else {
        for (const auto& product : products) {
            product.display();
        }
    }
}

void Store::displaySellers() const {
    cout << "\n=== SELLERS ===" << endl;
    if (sellers.empty()) {
        cout << "No sellers available." << endl;
    } else {
        for (const auto& seller : sellers) {
            seller.display();
        }
    }
}

void Store::displayBuyers() const {
    cout << "\n=== BUYERS ===" << endl;
    if (buyers.empty()) {
        cout << "No buyers available." << endl;
    } else {
        for (const auto& buyer : buyers) {
            buyer.display();
        }
    }
}

void Store::displayFinancialReport() const {
    cout << "\n=== FINANCIAL REPORT ===" << endl;
    cout << "Current Balance: " << fixed << setprecision(2) << balance << " USD" << endl;

    double totalSalesRevenue = 0;
    double totalStoreProfit = 0;
    int totalItemsSold = 0;

    for (const auto& seller : sellers) {
        totalStoreProfit += seller.getTotalProfit();
        totalItemsSold += seller.getItemsSold();
    }

    // Загальний дохід від продажів - це сума всіх грошей, отриманих від покупців
    // Для спрощення вважаємо, що це баланс мінус початковий баланс
    // Але краще було б вести окремий лічильник
    totalSalesRevenue = balance - 15000.0; // Початковий баланс

    cout << "Total Sales Revenue: " << totalSalesRevenue << " USD" << endl;
    cout << "Total Store Profit: " << totalStoreProfit << " USD" << endl;
    cout << "Total Items Sold: " << totalItemsSold << endl;

    double totalSalaries = 0;
    for (const auto& seller : sellers) {
        totalSalaries += seller.getSalary();
    }
    cout << "Total Salaries: " << totalSalaries << " USD" << endl;

    double netProfit = totalStoreProfit - totalSalaries;
    cout << "Net Profit: " << netProfit << " USD" << endl;
}

double Store::getBalance() const {
    return balance;
}

vector<Product> Store::getProducts() const {
    return products;
}

vector<Seller> Store::getSellers() const {
    return sellers;
}

vector<Buyer> Store::getBuyers() const {
    return buyers;
}

// Метод для ручного введення покупок з можливістю повтору при невдачі
void Store::manualPurchaseInput() {
    if (products.empty() || sellers.empty() || buyers.empty()) {
        cout << "Cannot make purchase: need products, sellers and buyers" << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Показуємо доступних покупців
    cout << "\nAvailable buyers:" << endl;
    for (size_t i = 0; i < buyers.size(); ++i) {
        cout << i + 1 << ". " << buyers[i].getName() << " (Discount: " << buyers[i].getDiscount() * 100 << "%)" << endl;
    }

    int buyerChoice;
    cout << "Select buyer (enter number): ";
    cin >> buyerChoice;

    if (buyerChoice < 1 || buyerChoice > static_cast<int>(buyers.size())) {
        cout << "Invalid buyer selection!" << endl;
        return;
    }
    string buyerName = buyers[buyerChoice - 1].getName();

    // Показуємо доступних продавців
    cout << "\nAvailable sellers:" << endl;
    for (size_t i = 0; i < sellers.size(); ++i) {
        cout << i + 1 << ". " << sellers[i].getName() << endl;
    }

    int sellerChoice;
    cout << "Select seller (enter number): ";
    cin >> sellerChoice;

    if (sellerChoice < 1 || sellerChoice > static_cast<int>(sellers.size())) {
        cout << "Invalid seller selection!" << endl;
        return;
    }
    string sellerName = sellers[sellerChoice - 1].getName();

    // Цикл для вибору товару та кількості з можливістю повтору
    bool continueShopping = true;
    while (continueShopping) {
        // Показуємо доступні товари
        cout << "\nAvailable products:" << endl;
        for (size_t i = 0; i < products.size(); ++i) {
            cout << i + 1 << ". " << products[i].getName()
                  << " (Price: " << products[i].getPrice()
                  << ", Available: " << products[i].getQuantity()
                  << ", Profit per item: " << products[i].getProfitPerItem() << ")" << endl;
        }

        int productChoice;
        cout << "Select product (enter number, or 0 to finish): ";
        cin >> productChoice;

        if (productChoice == 0) {
            continueShopping = false;
            break;
        }

        if (productChoice < 1 || productChoice > static_cast<int>(products.size())) {
            cout << "Invalid product selection!" << endl;
            continue;
        }
        string productName = products[productChoice - 1].getName();
        Product* selectedProduct = findProduct(productName);

        if (!selectedProduct) {
            cout << "Product not found!" << endl;
            continue;
        }

        // Цикл для введення кількості з можливістю повтору при невдачі
        bool quantityValid = false;
        while (!quantityValid) {
            int quantity;
            cout << "Enter quantity for " << productName << " (available: " << selectedProduct->getQuantity() << "): ";
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Quantity must be positive!" << endl;
                continue;
            }

            if (purchaseItems(buyerName, sellerName, productName, quantity)) {
                cout << "Purchase completed successfully! " << quantity << " x " << productName << endl;
                quantityValid = true;

                // Питаємо чи хоче користувач продовжити покупки
                char continueChoice;
                cout << "Do you want to continue shopping? (y/n): ";
                cin >> continueChoice;

                if (continueChoice == 'n' || continueChoice == 'N') {
                    continueShopping = false;
                }
            } else {
                cout << "Purchase failed! Not enough quantity of " << productName << endl;
                cout << "Available: " << selectedProduct->getQuantity() << endl;

                char retryChoice;
                cout << "Do you want to try different quantity? (y/n): ";
                cin >> retryChoice;

                if (retryChoice == 'n' || retryChoice == 'N') {
                    quantityValid = true; // Виходимо з циклу кількості, але продовжуємо shopping
                }
            }
        }
    }

    cout << "Shopping completed!" << endl;
}

// Реалізація допоміжних функцій
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        } else {
            cout << "Invalid input. Please enter a number." << endl;
            clearInput();
        }
    }
}

double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        } else {
            cout << "Invalid input. Please enter a number." << endl;
            clearInput();
        }
    }
}

string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

char getCharInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input.empty() ? ' ' : input[0];
}

void initializeStoreWithSampleData(Store& store) {
    cout << "\n=== INITIALIZING STORE WITH SAMPLE DATA ===" << endl;

    // Додавання товарів (ціна, собівартість, кількість)
    store.addProduct(Product("Apple", 1.5, 0.8, 50));   // Прибуток: 0.7 на одиницю
    store.addProduct(Product("Bread", 2.0, 1.0, 30));    // Прибуток: 1.0 на одиницю
    store.addProduct(Product("Milk", 3.0, 1.5, 20));     // Прибуток: 1.5 на одиницю
    store.addProduct(Product("Cheese", 5.0, 3.0, 15));   // Прибуток: 2.0 на одиницю
    store.addProduct(Product("Chocolate", 4.5, 2.5, 25));// Прибуток: 2.0 на одиницю

    // Додавання продавців
    store.addSeller(Seller("Alice Smith", "alice@store.com"));
    store.addSeller(Seller("Bob Johnson", "bob@store.com"));
    store.addSeller(Seller("Carol Williams", "carol@store.com"));

    // Додавання покупців (ім'я, контакт, знижка, постійний клієнт)
    store.addBuyer(Buyer("Charlie Brown", "charlie@email.com", 0.0, false));
    store.addBuyer(Buyer("Diana Prince", "diana@email.com", 0.1, true));
    store.addBuyer(Buyer("Bruce Wayne", "bruce@wayne.com", 0.15, true));
    store.addBuyer(Buyer("Clark Kent", "clark@dailyplanet.com", 0.05, false));

    cout << "Store initialized with sample data:" << endl;
    cout << "- 5 products with profit margins" << endl;
    cout << "- 3 sellers" << endl;
    cout << "- 4 buyers with different discounts" << endl;
}

void manualDataInput(Store& store) {
    cout << "\n=== MANUAL DATA INPUT ===" << endl;

    // Введення товарів
    cout << "\n--- PRODUCTS INPUT ---" << endl;
    int productCount = getIntInput("How many products do you want to add? ");

    for (int i = 0; i < productCount; i++) {
        cout << "\nProduct #" << (i + 1) << ":" << endl;
        string name = getStringInput("Product name: ");
        double price = getDoubleInput("Price: ");
        double cost = getDoubleInput("Cost: ");
        int quantity = getIntInput("Quantity: ");

        store.addProduct(Product(name, price, cost, quantity));
        cout << "Product '" << name << "' added successfully! Profit per item: " << (price - cost) << " USD" << endl;
    }

    // Введення продавців
    cout << "\n--- SELLERS INPUT ---" << endl;
    int sellerCount = getIntInput("How many sellers do you want to add? ");

    for (int i = 0; i < sellerCount; i++) {
        cout << "\nSeller #" << (i + 1) << ":" << endl;
        string name = getStringInput("Seller name: ");
        string contact = getStringInput("Contact: ");

        store.addSeller(Seller(name, contact));
        cout << "Seller '" << name << "' added successfully!" << endl;
    }

    // Введення покупців
    cout << "\n--- BUYERS INPUT ---" << endl;
    int buyerCount = getIntInput("How many buyers do you want to add? ");

    for (int i = 0; i < buyerCount; i++) {
        cout << "\nBuyer #" << (i + 1) << ":" << endl;
        string name = getStringInput("Buyer name: ");
        string contact = getStringInput("Contact: ");
        double discount = getDoubleInput("Discount (0-1): ");
        char regular = getCharInput("Regular customer? (y/n): ");

        store.addBuyer(Buyer(name, contact, discount, (regular == 'y' || regular == 'Y')));
        cout << "Buyer '" << name << "' added successfully!" << endl;
    }

    cout << "\nManual data input completed!" << endl;
}

void manualPurchasesInput(Store& store) {
    cout << "\n=== MANUAL PURCHASES INPUT ===" << endl;

    int purchaseCount = getIntInput("How many purchase sessions do you want to enter? ");

    for (int i = 0; i < purchaseCount; i++) {
        cout << "\n--- Purchase Session #" << (i + 1) << " ---" << endl;
        store.manualPurchaseInput();
    }

    cout << "\nManual purchases input completed!" << endl;
}

void simulateDay(Store& store, int day, bool automaticMode) {
    cout << "\n\n=== DAY " << day << " SIMULATION ===" << endl;

    if (automaticMode) {
        // Випадкові покупки тільки в автоматичному режимі
        cout << "\n1. Simulating customer purchases..." << endl;
        int transactions = 10 + (rand() % 15); // Більше транзакцій
        store.simulateRandomPurchases(transactions);
        this_thread::sleep_for(chrono::seconds(1));

        // Випадкове поповнення запасів
        cout << "\n2. Restocking products..." << endl;
        int restocks = 2 + (rand() % 4);
        for (int i = 0; i < restocks; i++) {
            store.simulateRandomRestocking();
        }
    } else {
        // В ручному режимі - тільки розрахунок зарплат
        cout << "\nProcessing daily operations..." << endl;
    }

    // Розрахунок зарплат
    cout << "\n3. Calculating salaries..." << endl;
    store.calculateSalaries(0.05); // 5% від прибутку
    this_thread::sleep_for(chrono::seconds(1));

    // Показ результатів дня
    cout << "\n4. End of day report:" << endl;
    store.displayFinancialReport();
}