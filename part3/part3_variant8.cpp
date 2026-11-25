// Частина 3. Варіант 8.
// Умова: відсортувати КОПІЮ вектора за зростанням, не змінюючи початковий вектор.
//
// Чиста функція: параметр приймається за const&, тому змінити його неможливо
// (компілятор не дозволить); сортується локальна копія, яку повертаємо за значенням.
// Параметр можна було б прийняти й за значенням (копія створилась би автоматично),
// але вимога завдання - const std::vector<int>&.

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> sorted_copy(const std::vector<int>& v) {
    std::vector<int> copy = v;                 // копія; оригінал не чіпаємо
    std::sort(copy.begin(), copy.end());
    return copy;
}

void print(const char* title, const std::vector<int>& v) {
    std::cout << title << ": ";
    if (v.empty()) std::cout << "(порожньо)";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';
}

int main() {
    const std::vector<int> original = {5, -1, 3, 3, 0, 9, -7, 2};
    const std::vector<int> snapshot = original;      // для автоматичної перевірки

    print("Початковий вектор до виклику   ", original);
    const auto result = sorted_copy(original);
    print("Результат (відсортована копія) ", result);
    print("Початковий вектор після виклику", original);
    std::cout << "Початковий вектор не змінився: "
              << (original == snapshot ? "так" : "НІ") << '\n';
    std::cout << "Результат відсортований:       "
              << (std::is_sorted(result.begin(), result.end()) ? "так" : "НІ") << "\n\n";

    // Порожній вектор
    const std::vector<int> empty;
    print("Порожній вектор до виклику     ", empty);
    const auto emptyResult = sorted_copy(empty);
    print("Результат для порожнього       ", emptyResult);
    print("Порожній вектор після виклику  ", empty);
}
