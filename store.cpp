#include "store.h"

// Реалізація методів серіалізації для Person
void Person::serialize(ofstream& file) const {
    size_t nameSize = name.size();
    file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    file.write(name.c_str(), nameSize);

    size_t contactSize = contact.size();
    file.write(reinterpret_cast<const char*>(&contactSize), sizeof(contactSize));
    file.write(contact.c_str(), contactSize);
}

void Person::deserialize(ifstream& file) {
    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    file.read(&name[0], nameSize);

    size_t contactSize;
    file.read(reinterpret_cast<char*>(&contactSize), sizeof(contactSize));
    contact.resize(contactSize);
    file.read(&contact[0], contactSize);
}

// Реалізація методів класу Person
Person::Person(const string& n, const string& c) : name(n), contact(c) {}

void Person::display() const {
    cout << "Name: " << name << ", Contact: " << contact;
}

string Person::getName() const { return name; }
string Person::getContact() const { return contact; }

// Реалізація серіалізації для Buyer
void Buyer::serialize(ofstream& file) const {
    Person::serialize(file);
    file.write(reinterpret_cast<const char*>(&discount), sizeof(discount));
    file.write(reinterpret_cast<const char*>(&purchasesCount), sizeof(purchasesCount));
    file.write(reinterpret_cast<const char*>(&isRegular), sizeof(isRegular));
    file.write(reinterpret_cast<const char*>(&totalSpent), sizeof(totalSpent));
}

void Buyer::deserialize(ifstream& file) {
    Person::deserialize(file);
    file.read(reinterpret_cast<char*>(&discount), sizeof(discount));
    file.read(reinterpret_cast<char*>(&purchasesCount), sizeof(purchasesCount));
    file.read(reinterpret_cast<char*>(&isRegular), sizeof(isRegular));
    file.read(reinterpret_cast<char*>(&totalSpent), sizeof(totalSpent));
}

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

// Реалізація серіалізації для Seller
void Seller::serialize(ofstream& file) const {
    Person::serialize(file);
    file.write(reinterpret_cast<const char*>(&salary), sizeof(salary));
    file.write(reinterpret_cast<const char*>(&totalProfit), sizeof(totalProfit));
    file.write(reinterpret_cast<const char*>(&itemsSold), sizeof(itemsSold));
}

void Seller::deserialize(ifstream& file) {
    Person::deserialize(file);
    file.read(reinterpret_cast<char*>(&salary), sizeof(salary));
    file.read(reinterpret_cast<char*>(&totalProfit), sizeof(totalProfit));
    file.read(reinterpret_cast<char*>(&itemsSold), sizeof(itemsSold));
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

// Реалізація серіалізації для Manager
void Manager::serialize(ofstream& file) const {
    Person::serialize(file);
}

void Manager::deserialize(ifstream& file) {
    Person::deserialize(file);
}

// Реалізація методів класу Manager
Manager::Manager(const string& n, const string& c) : Person(n, c) {}

void Manager::display() const {
    cout << "Manager - ";
    Person::display();
    cout << endl;
}

// Реалізація серіалізації для Product
void Product::serialize(ofstream& file) const {
    size_t nameSize = name.size();
    file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    file.write(name.c_str(), nameSize);

    file.write(reinterpret_cast<const char*>(&price), sizeof(price));
    file.write(reinterpret_cast<const char*>(&cost), sizeof(cost));
    file.write(reinterpret_cast<const char*>(&quantity), sizeof(quantity));
    file.write(reinterpret_cast<const char*>(&soldQuantity), sizeof(soldQuantity));
}

void Product::deserialize(ifstream& file) {
    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    file.read(&name[0], nameSize);

    file.read(reinterpret_cast<char*>(&price), sizeof(price));
    file.read(reinterpret_cast<char*>(&cost), sizeof(cost));
    file.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
    file.read(reinterpret_cast<char*>(&soldQuantity), sizeof(soldQuantity));
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
    return price - cost;
}

void Product::display() const {
    cout << "Product: " << name << ", Price: " << fixed << setprecision(2)
         << getPrice() << " USD, Cost: " << cost << " USD, Quantity: " << quantity
         << ", Sold: " << soldQuantity << ", Profit per item: " << getProfitPerItem() << " USD" << endl;
}

// Реалізація методів класу Store
Store::Store(const Manager& m, double initialBalance)
    : manager(m), balance(initialBalance), autoSave(false) {
    loadFromBinaryFile();
}

Store::~Store() {
    saveToBinaryFile();
}

void Store::saveToBinaryFile() {
    try {
        // Зберігаємо продукти
        ofstream productFile("products.bin", ios::binary);
        for (const auto& product : products) {
            product.serialize(productFile);
        }
        productFile.close();

        // Зберігаємо продавців
        ofstream sellerFile("sellers.bin", ios::binary);
        for (const auto& seller : sellers) {
            seller.serialize(sellerFile);
        }
        sellerFile.close();

        // Зберігаємо покупців
        ofstream buyerFile("buyers.bin", ios::binary);
        for (const auto& buyer : buyers) {
            buyer.serialize(buyerFile);
        }
        buyerFile.close();

        // Зберігаємо баланс
        ofstream balanceFile("balance.bin", ios::binary);
        balanceFile.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
        balanceFile.close();

        // Зберігаємо менеджера
        ofstream managerFile("manager.bin", ios::binary);
        manager.serialize(managerFile);
        managerFile.close();

        cout << "Data saved to binary files successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error saving data: " << e.what() << endl;
    }
}

void Store::loadFromBinaryFile() {
    try {
        // Завантажуємо продукти
        ifstream productFile("products.bin", ios::binary);
        if (productFile.is_open()) {
            products.clear();
            while (productFile.peek() != EOF) {
                Product product;
                product.deserialize(productFile);
                if (!product.getName().empty()) {
                    products.push_back(product);
                }
            }
            productFile.close();
        }

        // Завантажуємо продавців
        ifstream sellerFile("sellers.bin", ios::binary);
        if (sellerFile.is_open()) {
            sellers.clear();
            while (sellerFile.peek() != EOF) {
                Seller seller;
                seller.deserialize(sellerFile);
                if (!seller.getName().empty()) {
                    sellers.push_back(seller);
                }
            }
            sellerFile.close();
        }

        // Завантажуємо покупців
        ifstream buyerFile("buyers.bin", ios::binary);
        if (buyerFile.is_open()) {
            buyers.clear();
            while (buyerFile.peek() != EOF) {
                Buyer buyer;
                buyer.deserialize(buyerFile);
                if (!buyer.getName().empty()) {
                    buyers.push_back(buyer);
                }
            }
            buyerFile.close();
        }

        // Завантажуємо баланс
        ifstream balanceFile("balance.bin", ios::binary);
        if (balanceFile.is_open()) {
            balanceFile.read(reinterpret_cast<char*>(&balance), sizeof(balance));
            balanceFile.close();
        }

        // Завантажуємо менеджера
        ifstream managerFile("manager.bin", ios::binary);
        if (managerFile.is_open()) {
            manager.deserialize(managerFile);
            managerFile.close();
        }

        cout << "Data loaded from binary files successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error loading data: " << e.what() << endl;
    }
}

void Store::appendProductToFile(const Product& product) {
    ofstream file("products.bin", ios::binary | ios::app);
    product.serialize(file);
    file.close();
}

void Store::appendSellerToFile(const Seller& seller) {
    ofstream file("sellers.bin", ios::binary | ios::app);
    seller.serialize(file);
    file.close();
}

void Store::appendBuyerToFile(const Buyer& buyer) {
    ofstream file("buyers.bin", ios::binary | ios::app);
    buyer.serialize(file);
    file.close();
}

void Store::addProduct(const Product& p) {
    products.push_back(p);
    if (autoSave) {
        appendProductToFile(p);
    }
}

void Store::addSeller(const Seller& s) {
    sellers.push_back(s);
    if (autoSave) {
        appendSellerToFile(s);
    }
}

void Store::addBuyer(const Buyer& b) {
    buyers.push_back(b);
    if (autoSave) {
        appendBuyerToFile(b);
    }
}

void Store::clearData() {
    products.clear();
    sellers.clear();
    buyers.clear();
    balance = 15000.0;

    // Видаляємо файли
    remove("products.bin");
    remove("sellers.bin");
    remove("buyers.bin");
    remove("balance.bin");
    remove("manager.bin");
}

// Решта методів Store залишаються без змін (вони в попередній відповіді)

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
            finalPrice *= 0.9;
        }

        double profitPerItem = product->getProfitPerItem();
        double totalProfitForSeller = profitPerItem * quantity;

        product->reduceQuantity(quantity);
        product->addSoldQuantity(quantity);
        balance += finalPrice;
        seller->addSale(totalProfitForSeller, quantity);
        buyer->incrementPurchases();
        buyer->addToTotalSpent(finalPrice);

        if (buyer->getPurchasesCount() >= 3 && !buyer->getIsRegular()) {
            buyer->setIsRegular(true);
        }

        if (autoSave) {
            saveToBinaryFile();
        }

        return true;

    } catch (const exception& e) {
        return false;
    }
}

void Store::restockProduct(const string& productName, int quantity) {
    Product* product = findProduct(productName);
    if (product) {
        product->addQuantity(quantity);
        if (autoSave) {
            saveToBinaryFile();
        }
    }
}

void Store::calculateSalaries(double profitShare) {
    for (auto& seller : sellers) {
        seller.calculateSalary(profitShare);
    }
    if (autoSave) {
        saveToBinaryFile();
    }
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

    double totalSales = 0;
    int totalItemsSold = 0;
    for (const auto& seller : sellers) {
        totalSales += seller.getTotalProfit();
        totalItemsSold += seller.getItemsSold();
    }

    cout << "Total Sales: " << totalSales << " USD" << endl;
    cout << "Total Items Sold: " << totalItemsSold << endl;

    double totalSalaries = 0;
    for (const auto& seller : sellers) {
        totalSalaries += seller.getSalary();
    }
    cout << "Total Salaries: " << totalSalaries << " USD" << endl;

    double profit = totalSales - totalSalaries;
    cout << "Net Profit: " << profit << " USD" << endl;
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
                    quantityValid = true;
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

    // Додавання товарів
    store.addProduct(Product("Apple", 1.5, 0.8, 50));
    store.addProduct(Product("Bread", 2.0, 1.0, 30));
    store.addProduct(Product("Milk", 3.0, 1.5, 20));
    store.addProduct(Product("Cheese", 5.0, 3.0, 15));
    store.addProduct(Product("Chocolate", 4.5, 2.5, 25));

    // Додавання продавців
    store.addSeller(Seller("Alice Smith", "alice@store.com"));
    store.addSeller(Seller("Bob Johnson", "bob@store.com"));
    store.addSeller(Seller("Carol Williams", "carol@store.com"));

    // Додавання покупців
    store.addBuyer(Buyer("Charlie Brown", "charlie@email.com", 0.0, false));
    store.addBuyer(Buyer("Diana Prince", "diana@email.com", 0.1, true));
    store.addBuyer(Buyer("Bruce Wayne", "bruce@wayne.com", 0.15, true));
    store.addBuyer(Buyer("Clark Kent", "clark@dailyplanet.com", 0.05, false));

    cout << "Store initialized with sample data:" << endl;
    cout << "- 5 products" << endl;
    cout << "- 3 sellers" << endl;
    cout << "- 4 buyers" << endl;
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
        int transactions = 10 + (rand() % 15);
        store.simulateRandomPurchases(transactions);

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
    store.calculateSalaries(0.05);

    // Показ результатів дня
    cout << "\n4. End of day report:" << endl;
    store.displayFinancialReport();
}