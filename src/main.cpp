#include <iostream>
#include <vector>

#include "strie.h"

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> S(n);
    for (auto &s: S) std::cin >> s;
    strie st(n, m, S);
    return 0;
}
