
// Godbolt: https://godbolt.org/z/YPzfoGx8d
// Quick-bench: https://quick-bench.com/q/yfaKccmMeOqubTIWJvYWNPxHfeg

#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

auto itt(auto n) {
    auto vec = std::vector<int>(n);
    std::iota(vec.begin(), vec.end(), 0);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](auto e) { return e * 2; });
    std::transform(vec.begin(), vec.end(), vec.begin(), [](auto e) { return e + 1; });
    return vec;
}

auto it(auto n) {
    auto vec = std::vector<int>(n);
    std::iota(vec.begin(), vec.end(), 0);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](auto e) { return (e * 2) + 1; });
    return vec;
}

auto g(auto n) {
    auto vec = std::vector<int>(n);
    std::generate(vec.begin(), vec.end(), [i = -1]() mutable { return (++i * 2) + 1; });
    return vec;
}

void print(auto v) {
    for (auto e : v) std::cout << e << ' ';
    std::cout << '\n';
}

auto main() -> int {

    print(itt(10));
    print(it(10));
    print(g(10));

    return 0;
}
