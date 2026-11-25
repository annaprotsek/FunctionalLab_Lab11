// Частина 1. Варіант 8.
// Умова: вивести лише непарні елементи вектора.
//
// Вибір алгоритмів:
//  * std::copy_if  - відбирає елементи за предикатом і не змінює вхідний вектор.
//                    Це саме "фільтрація": результат записується в новий вектор.
//  * std::for_each - використовується для виведення (вимога завдання).
// Предикат x % 2 != 0 коректний і для від'ємних чисел: у C++ (-3) % 2 == -1,
// тому перевірка "!= 0" працює, а перевірка "== 1" - ні.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// Чиста функція: не змінює вхід, не виводить нічого, повертає новий вектор.
std::vector<int> odd_elements(const std::vector<int>& v) {
    std::vector<int> result;
    std::copy_if(v.begin(), v.end(), std::back_inserter(result),
                 [](int x) { return x % 2 != 0; });
    return result;
}

// Виведення відокремлено від обчислень.
void print(const std::string& title, const std::vector<int>& v) {
    std::cout << title << ": ";
    if (v.empty()) {
        std::cout << "(порожньо)";
    }
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';
}

int main() {
    const std::vector<std::vector<int>> tests = {
        {1, 2, 3, 4, 5, 6, 7},        // змішаний
        {-3, -2, -1, 0, 1, 2},        // від'ємні числа
        {2, 4, 6},                    // непарних немає
        {}                            // порожній вектор
    };

    for (const auto& v : tests) {
        print("Вхідний вектор   ", v);
        print("Непарні елементи ", odd_elements(v));
        std::cout << '\n';
    }
}
