// Частина 2. Варіант 8. Функтор Power.
// Умова: функтор, який підносить передане число до заданого степеня p
// (p - невід'ємне ціле). Для уникнення переповнення тестові значення
// підібрано так, щоб результат вміщався в вибраний тип long long.
//
// Порівняння функтора й лямбда-виразу:
//  * Функтор - окремий іменований клас: його можна оголосити в заголовку,
//    повторно використовувати, мати кілька конструкторів і методів, а також
//    стан (у Power стану, який змінюється, немає: p_ лише зчитується,
//    тому operator() позначено const - це "чиста" операція).
//  * Лямбда - анонімний тип, що створюється на місці використання.
//    Захоплення [p] робить із компілятором те саме, що ми пишемо вручну:
//    створює клас із полем p та operator().
// Обидва способи передаються в std::transform однаково і дають однаковий результат.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

class Power {
public:
    explicit Power(unsigned p) : p_(p) {}

    long long operator()(int x) const {
        long long result = 1;                 // x^0 = 1 (у т.ч. 0^0 за домовленістю)
        for (unsigned i = 0; i < p_; ++i) {
            result *= x;
        }
        return result;
    }

private:
    unsigned p_;
};

// Чиста функція, яка застосовує будь-який викликаний об'єкт до вектора.
template <class F>
std::vector<long long> transform_all(const std::vector<int>& v, F f) {
    std::vector<long long> out;
    out.reserve(v.size());
    std::transform(v.begin(), v.end(), std::back_inserter(out), f);
    return out;
}

void print(const char* title, const std::vector<long long>& v) {
    std::cout << title << ": ";
    if (v.empty()) std::cout << "(порожньо)";
    std::for_each(v.begin(), v.end(), [](long long x) { std::cout << x << ' '; });
    std::cout << '\n';
}

int main() {
    const std::vector<int> data = {-3, -2, 0, 1, 2, 3, 10};

    for (unsigned p : {0u, 1u, 3u, 5u}) {
        // 1) функтор
        auto byFunctor = transform_all(data, Power(p));

        // 2) еквівалентний лямбда-вираз
        auto byLambda = transform_all(data, [p](int x) {
            long long result = 1;
            for (unsigned i = 0; i < p; ++i) {
                result *= x;
            }
            return result;
        });

        std::cout << "p = " << p << '\n';
        print("  функтор Power", byFunctor);
        print("  лямбда       ", byLambda);
        std::cout << "  результати збігаються: "
                  << (byFunctor == byLambda ? "так" : "НІ") << "\n\n";
    }

    // Порожній вектор
    auto empty = transform_all(std::vector<int>{}, Power(3));
    print("Порожній вхід, p = 3", empty);
}
