// Частина 6 (підвищена складність). Варіант 8: аналіз результативності гравців.
//
// Умова: відібрати гравців, які провели понад 10 матчів; обчислити середню кількість
// балів за матч (double); упорядкувати за спаданням середнього показника; вивести
// п'ять найрезультативніших (або всіх відібраних, якщо їх менше п'яти).
//
// Структура конвеєра:
//   players --filter(matches > 10)--> --transform(Player -> PlayerStat)--> [матеріалізація]
//           --sort(за avg спад., за name зрост.)--> --take(5)--> результат
//  * filter та transform - ледачі представлення (std::ranges), проміжних контейнерів немає;
//  * sort потребує контейнера з довільним доступом, тому тут ОДНА матеріалізація
//    (std::vector<PlayerStat>), а початковий контейнер не змінюється;
//  * take(5) - знову ледаче представлення над відсортованим вектором;
//  * при однаковому середньому порядок визначений: за іменем у алфавітному порядку
//    (відтворюваний результат).
// Чисті функції: average_per_match, to_stat, top_players. Виведення - лише в main.

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

struct Player {
    std::string name;
    int score;
    int matches;
};

struct PlayerStat {
    std::string name;
    double average;     // середня кількість балів за матч
};

// Чиста функція. Викликається лише для matches > 10, тому ділення на нуль неможливе;
// додаткова перевірка робить функцію безпечною самостійно.
constexpr double average_per_match(int score, int matches) {
    return matches > 0 ? static_cast<double>(score) / static_cast<double>(matches) : 0.0;
}

constexpr auto to_stat = [](const Player& p) {
    return PlayerStat{p.name, average_per_match(p.score, p.matches)};
};

constexpr auto has_enough_matches = [](const Player& p) { return p.matches > 10; };

// R&& - приймаємо будь-який діапазон гравців без копіювання та без зміни.
template <std::ranges::viewable_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, const Player&>
std::vector<PlayerStat> top_players(R&& players, std::size_t count) {
    // Ледача частина: відбір і перетворення (ще нічого не обчислено).
    auto stats = std::forward<R>(players)
               | std::views::filter(has_enough_matches)
               | std::views::transform(to_stat);

    // Матеріалізація, бо sort потребує контейнера.
    std::vector<PlayerStat> sorted;
    std::ranges::copy(stats, std::back_inserter(sorted));

    std::ranges::sort(sorted, [](const PlayerStat& a, const PlayerStat& b) {
        if (a.average != b.average) return a.average > b.average;   // спадання
        return a.name < b.name;                                     // визначений tie-break
    });

    // Перші count елементів (якщо їх менше - усі).
    const auto limit = std::min(count, sorted.size());
    sorted.resize(limit);
    return sorted;
}

void print(const std::string& title, const std::vector<PlayerStat>& result) {
    std::cout << title << '\n';
    if (result.empty()) {
        std::cout << "  (жодного гравця не відібрано)\n";
    }
    int place = 1;
    for (const auto& s : result) {
        std::cout << "  " << place++ << ". " << std::left << std::setw(10) << s.name
                  << std::right << std::fixed << std::setprecision(3) << s.average << '\n';
    }
}

int main() {
    const std::vector<Player> players = {
        {"Andrii", 240, 12}, {"Bohdan", 150, 15}, {"Carla", 300, 20},
        {"Dmytro", 90, 9},   {"Eva", 330, 22},    {"Fedir", 130, 11},
        {"Hanna", 100, 10},  {"Ivan", 275, 25},   {"Jana", 180, 12},
        {"Kyrylo", 55, 5},   {"Lesia", 260, 13}
    };
    const auto snapshot = players;

    print("Топ-5 гравців (понад 10 матчів):", top_players(players, 5));

    // Менше п'яти відібраних гравців: виводяться всі відібрані.
    const std::vector<Player> few = {{"Oksana", 120, 11}, {"Petro", 40, 3}, {"Roman", 99, 12}};
    print("\nМенше п'яти підходящих гравців:", top_players(few, 5));

    // Однакові середні показники -> порядок за іменем.
    const std::vector<Player> ties = {{"Zoya", 110, 11}, {"Adam", 110, 11}, {"Mykola", 220, 22}};
    print("\nОднаковий середній показник (tie-break за іменем):", top_players(ties, 5));

    // Порожній вхід та випадок, коли ніхто не пройшов фільтр.
    print("\nПорожній набір даних:", top_players(std::vector<Player>{}, 5));
    print("\nУсі мають <= 10 матчів:", top_players(std::vector<Player>{{"A", 10, 10}}, 5));

    const bool unchanged = std::ranges::equal(players, snapshot, [](const Player& a, const Player& b) {
        return a.name == b.name && a.score == b.score && a.matches == b.matches;
    });
    std::cout << "\nПочаткові дані не змінено: " << (unchanged ? "так" : "НІ") << '\n';
}
