// Частина 5, завдання 3: час життя тимчасових об'єктів і std::ranges::dangling.
//
// ПРИЧИНА помилки. make() повертає тимчасовий вектор. Він знищується в кінці повного
// виразу, тобто одразу після повернення з max_element. Ітератор, що вказував би на
// елемент цього вектора, став би висячим (dangling). Тому алгоритми std::ranges, отримавши
// rvalue-діапазон, який не є borrowed_range, повертають замість ітератора спеціальний
// тип std::ranges::dangling. Він не підтримує оператор *, і помилка виявляється
// ПІД ЧАС КОМПІЛЯЦІЇ, а не як невизначена поведінка під час виконання.
//
// ВИПРАВЛЕННЯ: зробити діапазон lvalue - зберегти результат make() у змінну, час життя
// якої довший за час використання ітератора. Альтернативи: std::ranges::max(make())
// повертає ЗНАЧЕННЯ (не ітератор), тому тимчасовий об'єкт не заважає.
#include <algorithm>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

std::vector<int> make() {
    return {3, 1, 4};
}

int main() {
    // Доказ: для тимчасового діапазону повертається саме std::ranges::dangling.
    static_assert(std::is_same_v<decltype(std::ranges::max_element(make())),
                                 std::ranges::dangling>);

    // Виправлення 1: іменований контейнер.
    const std::vector<int> data = make();
    auto it = std::ranges::max_element(data);
    if (it != data.end()) {
        std::cout << "max_element: " << *it << std::endl;          // 4
    }

    // Виправлення 2: алгоритм, що повертає значення, а не ітератор.
    std::cout << "ranges::max:  " << std::ranges::max(make()) << std::endl;   // 4

    // Порожній діапазон: max_element повертає end(), розіменовувати його не можна.
    const std::vector<int> empty;
    auto itEmpty = std::ranges::max_element(empty);
    std::cout << "порожній діапазон: "
              << (itEmpty == empty.end() ? "елемента немає" : "є елемент") << std::endl;
}
