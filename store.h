#ifndef STORE_H
#define STORE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <random>
#include <map>
#include <iomanip>
#include <limits>
#include <thread>
#include <chrono>

using namespace std;

// Базовий клас Людина
class Person {
protected:
    string name;
    string contact;
public:
    Person(const string& n = "", const string& c = "");
    virtual ~Person() = default;
    virtual void display() const;
    string getName() const;
    string getContact() const;

    // Методи для серіалізації
    virtual void serialize(ofstream& file) const;
    virtual void deserialize(ifstream& file);
};

// Клас Покупець
class Buyer : public Person {
private:
    double discount;
    int purchasesCount;
    bool isRegular;
    double totalSpent;
public:
    Buyer(const string& n = "", const string& c = "", double disc = 0.0, bool regular = false);

    double getDiscount() const;
    void incrementPurchases();
    int getPurchasesCount() const;
    bool getIsRegular() const;
    void setIsRegular(bool regular);
    void addToTotalSpent(double amount);
    double getTotalSpent() const;

    void display() const override;

    // Серіалізація
    void serialize(ofstream& file) const override;
    void deserialize(ifstream& file) override;
};

// Клас Продавець
class Seller : public Person {
private:
    double salary;
    double totalProfit;
    int itemsSold;
public:
    Seller(const string& n = "", const string& c = "");

    void calculateSalary(double profitShare);
    void addSale(double itemProfit, int quantity = 1);

    double getSalary() const;
    double getTotalProfit() const;
    int getItemsSold() const;

    void display() const override;

    // Серіалізація
    void serialize(ofstream& file) const override;
    void deserialize(ifstream& file) override;
};

// Клас Керівник
class Manager : public Person {
public:
    Manager(const string& n = "", const string& c = "");

    void display() const override;

    // Серіалізація
    void serialize(ofstream& file) const override;
    void deserialize(ifstream& file) override;
};

// Клас Товар
class Product {
private:
    string name;
    double price;
    double cost;
    int quantity;
    int soldQuantity;
public:
    Product(const string& n = "", double p = 0.0, double c = 0.0, int q = 0);

    double getPrice() const;
    double getCost() const;
    int getQuantity() const;
    string getName() const;

    void reduceQuantity(int count);
    void addQuantity(int count);

    int getSoldQuantity() const;
    void addSoldQuantity(int q);

    void display() const;
    double getProfitPerItem() const;

    // Серіалізація
    void serialize(ofstream& file) const;
    void deserialize(ifstream& file);
};

// Клас Магазин
class Store {
private:
    vector<Product> products;
    vector<Seller> sellers;
    vector<Buyer> buyers;
    Manager manager;
    double balance;
    bool autoSave; // Флаг автоматичного збереження

    Product* findProduct(const string& productName);
    Seller* findSeller(const string& sellerName);
    Buyer* findBuyer(const string& buyerName);

    // Допоміжні функції для роботи з файлами
    void saveToBinaryFile();
    void loadFromBinaryFile();
    void appendProductToFile(const Product& product);
    void appendSellerToFile(const Seller& seller);
    void appendBuyerToFile(const Buyer& buyer);

public:
    Store(const Manager& m, double initialBalance = 10000.0);
    ~Store();

    void setAutoSave(bool enable) { autoSave = enable; }
    bool getAutoSave() const { return autoSave; } // ДОДАНО МЕТОД GETTER

    void addProduct(const Product& p);
    void addSeller(const Seller& s);
    void addBuyer(const Buyer& b);

    bool purchaseItems(const string& buyerName, const string& sellerName,
                      const string& productName, int quantity);

    void restockProduct(const string& productName, int quantity);
    void calculateSalaries(double profitShare = 0.05);

    void simulateRandomPurchases(int numberOfTransactions);
    void simulateRandomRestocking();

    void displayStatus() const;
    void displayProducts() const;
    void displaySellers() const;
    void displayBuyers() const;
    void displayFinancialReport() const;

    double getBalance() const;
    vector<Product> getProducts() const;
    vector<Seller> getSellers() const;
    vector<Buyer> getBuyers() const;

    // Метод для ручного введення покупок з можливістю повтору при невдачі
    void manualPurchaseInput();

    // Очищення даних
    void clearData();
};

// Допоміжні функції для роботи з вводом
void clearInput();
int getIntInput(const string& prompt);
double getDoubleInput(const string& prompt);
string getStringInput(const string& prompt);
char getCharInput(const string& prompt);

// Функції для ініціалізації даних
void initializeStoreWithSampleData(Store& store);
void manualDataInput(Store& store);
void manualPurchasesInput(Store& store);
void simulateDay(Store& store, int day, bool automaticMode);

#endif