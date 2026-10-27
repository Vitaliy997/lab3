#ifndef MEDICINE_H
#define MEDICINE_H

#include <iostream>
#include <string>
#include <vector>
#include <exception>

// ---------- Ієрархія виняткових ситуацій ----------
class MedicineException : public std::exception {
protected:
    std::string message;
public:
    explicit MedicineException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

// Значення поза допустимими межами
class ValueRangeException : public MedicineException {
public:
    explicit ValueRangeException(const std::string& msg) : MedicineException(msg) {}
};

// Читання поля, якому ще не присвоєно значення
class NotInitializedException : public MedicineException {
public:
    explicit NotInitializedException(const std::string& msg) : MedicineException(msg) {}
};

// ---------- Клас ЛІКИ ----------
class Medicine {
private:
    std::string name;      // назва
    int    quantity;       // кількість одиниць товару
    double price;          // вартість однієї одиниці, грн
    bool   quantitySet;    // чи встановлена кількість
    bool   priceSet;       // чи встановлена вартість

public:
    static const int    MAX_QUANTITY;
    static const double MAX_PRICE;

    // Перевантажені конструктори
    Medicine();
    explicit Medicine(const std::string& name);
    Medicine(const std::string& name, int quantity, double price);

    // Доступ до полів (з перевіркою)
    void setQuantity(int q);
    void setPrice(double p);
    int    getQuantity() const;
    double getPrice() const;

    void setName(const std::string& n) { name = n; }
    const std::string& getName() const { return name; }

    // Перевантажені операції
    Medicine& operator+=(int units);        // збільшити кількість одиниць
    Medicine& operator-=(int units);        // зменшити кількість одиниць
    Medicine  operator+(double delta) const; // збільшити вартість
    Medicine  operator-(double delta) const; // зменшити вартість

    friend std::istream& operator>>(std::istream& in, Medicine& m);
    friend std::ostream& operator<<(std::ostream& out, const Medicine& m);

    // Перевантажені методи вартості ліків за рецептом
    double prescriptionCost() const;                          // 1 одиниця
    double prescriptionCost(int units) const;                 // units одиниць
    double prescriptionCost(int units, double discountPercent) const; // зі знижкою (%)

    // Перевантажені методи пошуку ліків за різними критеріями
    static std::vector<int> find(const std::vector<Medicine>& v, const std::string& name);
    static std::vector<int> find(const std::vector<Medicine>& v, const std::string& name,
        double maxPrice);
    static std::vector<int> find(const std::vector<Medicine>& v, int minQty, int maxQty);
    static std::vector<int> find(const std::vector<Medicine>& v, double minPrice, double maxPrice);
};

#endif