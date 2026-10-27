#include "Medicine.h"
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void printResult(const vector<Medicine>& v, const vector<int>& idx) {
    if (idx.empty()) { cout << "  Нічого не знайдено\n"; return; }
    for (int i : idx) cout << "  [" << i << "] " << v[i] << endl;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    vector<Medicine> meds;

    cout << "=== 1. Введення (operator>>) ===\n";
    istringstream data("Аспірин 120 45.50\n"
        "Парацетамол 300 32.00\n"
        "Цитрамон 80 28.75\n"
        "Аспірин 15 62.00\n");
    for (int i = 0; i < 4; ++i) {
        Medicine m;
        try { data >> m; meds.push_back(m); }
        catch (const MedicineException& e) { cout << "Помилка: " << e.what() << endl; }
    }
    for (const Medicine& m : meds) cout << m << endl;

    cout << "\n=== 2. Зміна кількості (+=, -=) ===\n";
    try {
        meds[0] += 30;
        cout << "Після Аспірин += 30: " << meds[0] << endl;
        meds[0] -= 50;
        cout << "Після Аспірин -= 50: " << meds[0] << endl;
        meds[3] -= 100;
    }
    catch (const ValueRangeException& e) { cout << "[ValueRange] " << e.what() << endl; }
    catch (const NotInitializedException& e) { cout << "[NotInit] " << e.what() << endl; }
    catch (const MedicineException& e) { cout << "[Medicine] " << e.what() << endl; }

    cout << "\n=== 3. Зміна вартості (+, -) ===\n";
    try {
        meds[1] = meds[1] + 8.0;
        cout << "Після Парацетамол + 8.00: " << meds[1] << endl;
        meds[1] = meds[1] - 3.5;
        cout << "Після Парацетамол - 3.50: " << meds[1] << endl;
        meds[2] = meds[2] - 40.0;
    }
    catch (const ValueRangeException& e) { cout << "[ValueRange] " << e.what() << endl; }
    catch (const MedicineException& e) { cout << "[Medicine] " << e.what() << endl; }

    cout << "\n=== 4. Вартість за рецептом ===\n";
    try {
        cout << fixed << setprecision(2);
        cout << "1 упаковка Цитрамону:            " << meds[2].prescriptionCost() << " грн\n";
        cout << "5 упаковок Цитрамону:            " << meds[2].prescriptionCost(5) << " грн\n";
        cout << "5 упаковок зі знижкою 20%:       " << meds[2].prescriptionCost(5, 20.0) << " грн\n";
        cout << "500 упаковок Цитрамону:          ";
        cout << meds[2].prescriptionCost(500) << " грн\n";
    }
    catch (const ValueRangeException& e) { cout << "\n[ValueRange] " << e.what() << endl; }
    cout << defaultfloat;

    cout << "\n=== 5. Пошук ===\n";
    cout << "За назвою Аспірин:\n";                       printResult(meds, Medicine::find(meds, "Аспірин"));
    cout << "За назвою Аспірин і ціною до 50 грн:\n";     printResult(meds, Medicine::find(meds, "Аспірин", 50.0));
    cout << "За кількістю 50..150 од.:\n";                printResult(meds, Medicine::find(meds, 50, 150));
    cout << "За вартістю 30.0..50.0 грн:\n";              printResult(meds, Medicine::find(meds, 30.0, 50.0));

    cout << "\n=== 6. Виняткові ситуації ===\n";
    try {
        Medicine m("Ношпа");
        m.setPrice(-10.0);
    }
    catch (const ValueRangeException& e) { cout << "[ValueRange] " << e.what() << endl; }

    try {
        Medicine m("Ношпа");
        cout << m.getQuantity() << endl;
    }
    catch (const NotInitializedException& e) { cout << "[NotInit] " << e.what() << endl; }

    try {
        Medicine m("Ношпа", 50, 250000.0);
    }
    catch (const MedicineException& e) { cout << "[Medicine] " << e.what() << endl; }

    try {
        istringstream bad("Нурофен abc 95.0");
        Medicine m; bad >> m;
    }
    catch (const MedicineException& e) { cout << "[Medicine] " << e.what() << endl; }

    try {
        try { Medicine::find(meds, 300, 100); }
        catch (const MedicineException& e) {
            cout << "[внутрішній обробник] " << e.what() << endl;
            throw;
        }
    }
    catch (const exception& e) { cout << "[зовнішній обробник] " << e.what() << endl; }

    cout << "\n=== 7. Введіть ліки (назва кількість вартість): ";
    Medicine user;
    try {
        if (cin >> user) { meds.push_back(user); cout << "Додано: " << user << endl; }
    }
    catch (const MedicineException& e) { cout << "Помилка: " << e.what() << endl; }

    return 0;
}