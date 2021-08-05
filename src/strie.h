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
constexpr char key_position = '$';


class strie {
public:
    enum builder {
        recursive,
        memory,
        dp
    };
private:
    std::vector<std::unordered_map<char, int>> trie;
    builder builder_;

    std::vector<std::vector<std::bitset<maxm>>> K;
    std::vector<std::vector<std::bitset<maxm>>> R;
    std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> C;
    std::vector<std::vector<int>> P;
    std::vector<std::vector<int>> opt;

    void built_KR(const std::vector<std::string> &S, int n, int m);

    void built_C(int n, int m, const std::vector<std::string> &S);

    void built_opt(int n, int m);


    std::bitset<maxm> K_recursive(int i, int j, const std::vector<std::string> &S);

    std::vector<std::pair<int, int>> C_recursive(int i, int j, int r, const std::vector<std::string> &S);

    int opt_recursive(int i, int j, const std::vector<std::string> &S);


    int built_trie(int i, int j, int n, int m, const std::vector<std::string> &S, std::bitset<maxm> set);

public:
    strie(int n, int m, const std::vector<std::string> &S, builder b);

    int get_size();

    void print(int, char, int);

    std::string to_string();

    friend std::ostream &operator<<(std::ostream &os, const strie &st);
};

int count_bits(std::bitset<maxm> bs, int m);

std::vector<std::pair<int, int>>
merge_intervals(const std::vector<std::pair<int, int>> &a, const std::vector<std::pair<int, int>> &b);

std::ostream &operator<<(std::ostream &os, strie &st);

#endif //SRC_STRIE_H