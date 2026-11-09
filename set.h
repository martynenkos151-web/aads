#include <cmath>
#include <cstddef>
#include <ostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

// Случайное целое в диапазоне [low, high] включительно.
inline int make_random(const int& low, const int& high) {
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_int_distribution<int>(low, high)(engine);
}

// Случайное вещественное число в полуинтервале [low, high).
inline double make_random(const double& low, const double& high) {
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_real_distribution<double>(low, high)(engine);
}

// Случайный float: генерируем в double и приводим к float.
inline float make_random(const float& low, const float& high) {
    return static_cast<float>(make_random(static_cast<double>(low),
        static_cast<double>(high)));
}

// Случайная строка: границы задают min/max длину.
inline std::string make_random(const std::string& low, const std::string& high) {
    static std::mt19937 engine{ std::random_device{}() };
    const std::size_t len = std::uniform_int_distribution<std::size_t>(
        low.size(), high.size())(engine);
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<int> pick(0, 25);
    std::string result;
    for (std::size_t i = 0; i < len; ++i) {
        result.push_back(alphabet[pick(engine)]);
    }
    return result;
}

// Случайная пара: каждое поле в своём диапазоне.
inline std::pair<int, double> make_random(const std::pair<int, double>& low,
    const std::pair<int, double>& high) {
    return { make_random(low.first, high.first),
            make_random(low.second, high.second) };
}
