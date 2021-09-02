
// Godbolt: https://godbolt.org/z/rz65Y9WqG
// Quick-bench: https://quick-bench.com/q/FtSz60mXv6t0D5QfXivQyU2ry7Q

#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

auto k(auto v) {
    std::partial_sum(v.begin(), v.end(), v.begin(), 
        [](auto a, auto b) { return std::max(a + b, b); });
    return *std::max_element(v.begin(), v.end());
}

auto k2(auto v) {
    return std::accumulate(v.begin(), v.end(), 
        std::pair{0, 0},
        [](auto p, auto b) { 
            auto const [m, a] = p;
            auto const temp = std::max(a + b, b);
            return std::pair{ std::max(m, temp), temp };
         }).first;
}

auto scan_reduce_lift(auto outer_op, auto inner_op) {
    return [&](auto p, auto b) { 
        auto const [acc1, acc2] = p;
        auto const temp = inner_op(acc2, b);
        return std::pair{ outer_op(acc2, temp), temp };
    };
}

auto k3(auto v) {
    auto op  = [](auto a, auto b) { return std::max(a + b, b); };
    auto max = [](auto a, auto b) { return std::max(a, b); };
    return std::accumulate(v.begin(), v.end(), 
        std::pair{0, 0},
        scan_reduce_lift(max, op)).first;
}

auto main() -> int {

    auto const v = std::vector{2, 4, -8, 1, 3, -1, 4, -6};

    std::cout << k (v) << '\n';
    std::cout << k2(v) << '\n';
    std::cout << k3(v) << '\n';

    return 0;
}