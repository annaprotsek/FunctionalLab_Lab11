// Частина 4. Варіант 8.
// Умова: вибрати від'ємні числа, обчислити їх абсолютні значення та знайти
// середнє арифметичне. Для абсолютного значення забезпечити коректну обробку
// крайніх значень типу int (розширення типу до long long). Якщо від'ємних
// чисел немає - результат відсутній (std::optional).
//
// Чому розширення типу: abs(INT_MIN) для 32-бітного int не вміщається в int
// (|-2147483648| = 2147483648 > INT_MAX), тому "-x" для int дає
// невизначену поведінку. Спочатку перетворюємо x у long long, потім змінюємо знак.

#include <algorithm>
#include <concepts>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <optional>
#include <ranges>
#include <vector>

// Абсолютне значення без переповнення.
constexpr long long abs_wide(int x) {
    return x < 0 ? -static_cast<long long>(x) : static_cast<long long>(x);
}

// ---------------------------------------------------------------------------
// А. Класична реалізація (STL-алгоритми, проміжні контейнери)
// ---------------------------------------------------------------------------
std::optional<double> average_abs_negatives_classic(const std::vector<int>& v) {
    // Етап 1: фільтрація (std::copy_if) -> ПРОМІЖНИЙ КОНТЕЙНЕР negatives.
    std::vector<int> negatives;
    std::copy_if(v.begin(), v.end(), std::back_inserter(negatives),
                 [](int x) { return x < 0; });

    if (negatives.empty()) {
        return std::nullopt;                    // від'ємних немає -> середнього немає
    }

    // Етап 2: перетворення (std::transform) -> ЩЕ ОДИН ПРОМІЖНИЙ КОНТЕЙНЕР.
    std::vector<long long> absolutes(negatives.size());
    std::transform(negatives.begin(), negatives.end(), absolutes.begin(), abs_wide);

    // Етап 3: агрегація (std::accumulate), початкове значення 0LL (тип long long!).
    const long long sum = std::accumulate(absolutes.begin(), absolutes.end(), 0LL);

    // Ділення в типі double.
    return static_cast<double>(sum) / static_cast<double>(absolutes.size());
}

// ---------------------------------------------------------------------------
// Б. Реалізація з std::ranges (ледачі представлення, без проміжних контейнерів)
// ---------------------------------------------------------------------------
// R&& - forwarding reference: приймає і lvalue-контейнер, і тимчасовий діапазон.
template <std::ranges::viewable_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, int>
std::optional<double> average_abs_negatives_ranges(R&& r) {
    // Етапи 1-2 лише ОПИСУЮТЬ конвеєр. Жодна перевірка чи обчислення тут не виконується:
    // filter та transform створюють представлення (views), які зберігають діапазон і
    // функцію, але не обробляють елементи.
    auto pipeline = std::forward<R>(r)
                  | std::views::filter([](int x) { return x < 0; })
                  | std::views::transform([](int x) { return abs_wide(x); })
                  | std::views::common;     // однакові типи begin/end для std::accumulate

    // Фактична фільтрація й перетворення відбуваються ПІД ЧАС ОБХОДУ:
    // перший обхід - підрахунок кількості, другий - сума в std::accumulate.
    const auto count = std::ranges::distance(pipeline);
    if (count == 0) {
        return std::nullopt;
    }
    const long long sum = std::accumulate(pipeline.begin(), pipeline.end(), 0LL);
    return static_cast<double>(sum) / static_cast<double>(count);
}

// ---------------------------------------------------------------------------
// Демонстрація "ледачості" (лише для навчання: лічильник - побічний ефект,
// у робочій логіці так робити не слід).
// ---------------------------------------------------------------------------
void demo_laziness(const std::vector<int>& v) {
    int predicateCalls = 0;
    auto pipeline = v
                  | std::views::filter([&predicateCalls](int x) { ++predicateCalls; return x < 0; })
                  | std::views::transform([](int x) { return abs_wide(x); });
    std::cout << "  після побудови конвеєра викликів предиката: " << predicateCalls << '\n';
    long long sum = 0;
    for (long long x : pipeline) sum += x;
    std::cout << "  після обходу конвеєра викликів предиката:   " << predicateCalls
              << " (розмір вектора " << v.size() << "), сума = " << sum << '\n';
}

void print_result(const char* name, const std::optional<double>& r) {
    std::cout << "  " << name << ": ";
    if (r) std::cout << std::fixed << std::setprecision(4) << *r; else std::cout << "(відсутнє)";
    std::cout << '\n';
}

int main() {
    const int INT_MIN_V = std::numeric_limits<int>::min();
    const std::vector<std::vector<int>> tests = {
        {5, -3, 8, -10, 0, -7, 2},                 // звичайний
        {5, -3, 8, INT_MIN_V, 0, -7, 2},           // крайнє значення INT_MIN
        {1, 2, 3},                                  // від'ємних немає
        {}                                          // порожній вектор
    };

    int n = 1;
    for (const auto& v : tests) {
        std::cout << "Тест " << n++ << ", елементів: " << v.size() << '\n';
        const auto classic = average_abs_negatives_classic(v);
        const auto viaRanges = average_abs_negatives_ranges(v);
        print_result("класична реалізація", classic);
        print_result("ranges-реалізація  ", viaRanges);
        std::cout << "  результати збігаються: "
                  << (classic == viaRanges ? "так" : "НІ") << "\n\n";
    }

    std::cout << "Демонстрація ледачих обчислень (тест 1):\n";
    demo_laziness(tests[0]);
}
