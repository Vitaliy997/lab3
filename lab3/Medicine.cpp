#include "Medicine.h"
#include <sstream>
#include <iomanip>

const int    Medicine::MAX_QUANTITY = 100000;
const double Medicine::MAX_PRICE = 100000.0;

Medicine::Medicine()
    : name(""), quantity(0), price(0.0), quantitySet(false), priceSet(false) {
}

Medicine::Medicine(const std::string& n)
    : name(n), quantity(0), price(0.0), quantitySet(false), priceSet(false) {
}

Medicine::Medicine(const std::string& n, int q, double p)
    : name(n), quantity(0), price(0.0), quantitySet(false), priceSet(false) {
    setQuantity(q);   // перевірка меж виконується в сеттерах
    setPrice(p);
}

void Medicine::setQuantity(int q) {
    if (q < 0 || q > MAX_QUANTITY) {
        std::ostringstream os;
        os << "Кількість має бути в межах 0.." << MAX_QUANTITY << " од., отримано: " << q;
        throw ValueRangeException(os.str());
    }
    quantity = q;
    quantitySet = true;
}

void Medicine::setPrice(double p) {
    if (p <= 0.0 || p > MAX_PRICE) {
        std::ostringstream os;
        os << "Вартість має бути в межах 0.01.." << MAX_PRICE << " грн, отримано: " << p;
        throw ValueRangeException(os.str());
    }
    price = p;
    priceSet = true;
}

int Medicine::getQuantity() const {
    if (!quantitySet)
        throw NotInitializedException("Кількість одиниць ліків ще не встановлена");
    return quantity;
}

double Medicine::getPrice() const {
    if (!priceSet)
        throw NotInitializedException("Вартість ліків ще не встановлена");
    return price;
}

Medicine& Medicine::operator+=(int units) {
    if (!quantitySet)
        throw NotInitializedException("Неможливо змінити кількість: початкове значення не встановлене");
    if (units < 0)
        throw ValueRangeException("Приріст кількості не може бути від'ємним");
    if (quantity + units > MAX_QUANTITY)
        throw ValueRangeException("Кількість після збільшення перевищить допустиму межу");
    quantity += units;
    return *this;
}

Medicine& Medicine::operator-=(int units) {
    if (!quantitySet)
        throw NotInitializedException("Неможливо змінити кількість: початкове значення не встановлене");
    if (units < 0)
        throw ValueRangeException("Зменшення кількості не може бути від'ємним");
    if (units > quantity)
        throw ValueRangeException("Недостатньо ліків на складі для зменшення кількості");
    quantity -= units;
    return *this;
}

Medicine Medicine::operator+(double delta) const {
    if (!priceSet)
        throw NotInitializedException("Неможливо змінити вартість: початкове значення не встановлене");
    if (delta < 0)
        throw ValueRangeException("Збільшення вартості не може бути від'ємним");
    if (price + delta > MAX_PRICE)
        throw ValueRangeException("Вартість після збільшення перевищить допустиму межу");
    Medicine res = *this;
    res.price += delta;
    return res;
}

Medicine Medicine::operator-(double delta) const {
    if (!priceSet)
        throw NotInitializedException("Неможливо змінити вартість: початкове значення не встановлене");
    if (delta < 0)
        throw ValueRangeException("Зменшення вартості не може бути від'ємним");
    if (delta >= price)
        throw ValueRangeException("Вартість після зменшення має залишатися додатною");
    Medicine res = *this;
    res.price -= delta;
    return res;
}

std::istream& operator>>(std::istream& in, Medicine& m) {
    std::string n;
    int q;
    double p;
    if (!(in >> n >> q >> p)) {
        if (!in.eof()) in.clear(std::ios::failbit);
        throw MedicineException("Помилка введення: очікується 'назва кількість вартість'");
    }
    Medicine tmp(n, q, p);
    m = tmp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Medicine& m) {
    out << m.name << ", ";
    if (!m.quantitySet) out << "кількість: н/в"; else out << m.quantity << " од.";
    out << ", ";
    if (!m.priceSet) out << "вартість: н/в";
    else out << std::fixed << std::setprecision(2) << m.price << " грн" << std::defaultfloat;
    return out;
}

// ---------- Вартість за рецептом ----------
double Medicine::prescriptionCost() const {
    return prescriptionCost(1);
}

double Medicine::prescriptionCost(int units) const {
    if (!priceSet)
        throw NotInitializedException("Вартість ліків ще не встановлена");
    if (!quantitySet)
        throw NotInitializedException("Кількість ліків ще не встановлена");
    if (units <= 0)
        throw ValueRangeException("Кількість одиниць за рецептом має бути додатною");
    if (units > quantity) {
        std::ostringstream os;
        os << "За рецептом потрібно " << units << " од., а в наявності лише " << quantity;
        throw ValueRangeException(os.str());
    }
    return units * price;
}

double Medicine::prescriptionCost(int units, double discountPercent) const {
    if (discountPercent < 0.0 || discountPercent > 100.0)
        throw ValueRangeException("Відсоток знижки має бути в межах 0..100");
    return prescriptionCost(units) * (1.0 - discountPercent / 100.0);
}

// ---------- Пошук ----------
std::vector<int> Medicine::find(const std::vector<Medicine>& v, const std::string& name) {
    std::vector<int> res;
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].name == name) res.push_back((int)i);
    return res;
}

std::vector<int> Medicine::find(const std::vector<Medicine>& v, const std::string& name,
    double maxPrice) {
    std::vector<int> res;
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].name == name && v[i].priceSet && v[i].price <= maxPrice)
            res.push_back((int)i);
    return res;
}

std::vector<int> Medicine::find(const std::vector<Medicine>& v, int minQty, int maxQty) {
    if (minQty > maxQty) throw ValueRangeException("Нижня межа кількості більша за верхню");
    std::vector<int> res;
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].quantitySet && v[i].quantity >= minQty && v[i].quantity <= maxQty)
            res.push_back((int)i);
    return res;
}

std::vector<int> Medicine::find(const std::vector<Medicine>& v, double minPrice, double maxPrice) {
    if (minPrice > maxPrice) throw ValueRangeException("Нижня межа вартості більша за верхню");
    std::vector<int> res;
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].priceSet && v[i].price >= minPrice && v[i].price <= maxPrice)
            res.push_back((int)i);
    return res;
}