// Частина 5, завдання 1: remove_if та видалення елементів.
//
// АНАЛІЗ початкового коду. Результат виконання: розмір 5, вміст "1 3 5 -4 5".
// Пояснення: std::remove_if - це алгоритм, який працює з ітераторами й не знає про
// контейнер, тому не може змінити його розмір. Він лише ПЕРЕСУВАЄ елементи, що
// залишаються, на початок діапазону (1 3 5), і повертає ітератор на новий "логічний"
// кінець. Елементи після нього мають невизначений ("валідний, але не гарантований")
// вміст - тут це залишки -4 і 5. Повернене значення в початковому коді відкинуто,
// а v.size() залишився 5.
//
// ВИПРАВЛЕННЯ: ідіома erase-remove - передати повернений ітератор у v.erase(...),
// яка вже фізично видаляє хвіст. У C++20 є коротший варіант std::erase_if(v, pred).
#include <algorithm>
#include <iostream>
#include <vector>

void print(const char* title, const std::vector<int>& v) {
    std::cout << title << " size=" << v.size() << " : ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    const auto isNegative = [](int x) { return x < 0; };

    // 1) початкова (помилкова) поведінка
    std::vector<int> broken{1, -2, 3, -4, 5};
    std::remove_if(broken.begin(), broken.end(), isNegative);
    print("початковий код        ", broken);

    // 2) erase-remove
    std::vector<int> fixed{1, -2, 3, -4, 5};
    fixed.erase(std::remove_if(fixed.begin(), fixed.end(), isNegative), fixed.end());
    print("erase + remove_if     ", fixed);

    // 3) std::erase_if (C++20)
    std::vector<int> fixed2{1, -2, 3, -4, 5};
    const auto removed = std::erase_if(fixed2, isNegative);
    print("std::erase_if         ", fixed2);
    std::cout << "видалено елементів: " << removed << '\n';

    // Порожній вектор
    std::vector<int> empty;
    empty.erase(std::remove_if(empty.begin(), empty.end(), isNegative), empty.end());
    print("порожній вектор       ", empty);
}
