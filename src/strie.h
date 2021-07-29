//
// Created by ceduardosq on 7/28/21.
//

#ifndef SRC_STRIE_H
#define SRC_STRIE_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>

constexpr int maxm = 1e2;

class strie {
    std::vector<std::unordered_map<char, int>> trie;
    std::vector<char> alphabet;

    std::vector<std::vector<std::bitset<maxm>>> K;
    std::vector<std::vector<std::bitset<maxm>>> R;
    std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> C;
    std::vector<std::vector<int>> P;
    std::vector<std::vector<int>> opt;

    void set_alphabet(const std::vector<std::string> &S);

    void built_KR(const std::vector<std::string> &S, int n, int m);

    void built_C(int n, int m, const std::vector<std::string> &S);

    void built_opt(int n, int m);

//    int opt_(int i, int j, int m);

    int count_bits(std::bitset<maxm>, int m);

public:
    strie(int n, int m, const std::vector<std::string> &S);

    friend std::ostream &operator<<(std::ostream &os, const strie &st);

    int get_size();
};

std::ostream &operator<<(std::ostream &os, const strie &st);

#endif //SRC_STRIE_H