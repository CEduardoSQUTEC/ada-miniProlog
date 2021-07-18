#include <iostream>
#include <vector>



int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> S(n);
    for (auto &s: S) std::cin >> s;
    return 0;
}
