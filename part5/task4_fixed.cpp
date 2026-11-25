// Частина 5, завдання 4: передавання діапазону у функцію.
//
// ПРИЧИНА помилки. Цикл range-for викликає r.begin(). Для const filter_view цього методу
// немає: begin() у filter_view не є const, бо під час першого виклику він знаходить перший
// елемент, що задовольняє предикат, і кешує його всередині представлення (щоб begin()
// працював за O(1) при повторних викликах). Тому змінюється внутрішній стан, і const-версія
// неможлива. transform_view над таким діапазоном також не може бути ітерований як const.
// Отже параметр "const R&" не підходить для універсального приймання views.
//
// ВИПРАВЛЕННЯ: forwarding reference R&& (приймає і lvalue, і rvalue без копіювання
// контейнера) з обмеженням std::ranges::input_range та вимогою, щоб елементи були
// числами; для порожнього діапазону повертаємо std::optional<double>.
#include <concepts>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ranges>
#include <vector>

template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, double>
std::optional<double> average(R&& r) {
    double sum = 0.0;
    std::size_t n = 0;

    for (auto&& x : r) {          // r - іменований lvalue, не const: begin() доступний
        sum += static_cast<double>(x);
        ++n;
    }

    if (n == 0) {
        return std::nullopt;
    }
    return sum / static_cast<double>(n);
}

void print(const char* title, const std::optional<double>& result) {
    std::cout << title;
    if (result) std::cout << *result; else std::cout << "(середнього немає)";
    std::cout << std::endl;
}

int main() {
    auto r = std::views::iota(1, 31)
           | std::views::filter([](int x) { return x % 3 == 0; })
           | std::views::transform([](int x) { return x * x; });

    print("середнє квадратів чисел, кратних 3: ", average(r));        // 346.5

    // порожній діапазон: жодне число не задовольняє умову
    auto none = std::views::iota(1, 3)
              | std::views::filter([](int x) { return x % 3 == 0; });
    print("порожній діапазон:                  ", average(none));

    // тимчасовий діапазон і звичайний контейнер
    print("тимчасовий діапазон iota(1, 5):     ", average(std::views::iota(1, 5)));
    const std::vector<int> v{2, 4, 9};
    print("const vector<int>:                  ", average(v));
}
