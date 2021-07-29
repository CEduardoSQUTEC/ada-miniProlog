//
// Created by ceduardosq on 7/28/21.
//

#include "strie.h"
#include <iostream>
#include <iomanip>

void strie::set_alphabet(const std::vector<std::string> &S) { // O(nm)
    std::unordered_set<char> L;
    for (const auto &s: S)
        for (const auto &c: s)
            L.insert(c);
    alphabet.reserve(L.size());
    for (const auto &e: L) alphabet.push_back(e);
}

void strie::built_KR(const std::vector<std::string> &S, int n, int m) { // O(n^2m)
    K.resize(n, std::vector<std::bitset<maxm>>(n));
    R.resize(n, std::vector<std::bitset<maxm>>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            K[i][i][j] = true;
    for (int i = 0; i < n - 1; ++i) {
        for (int k = 0; k < m; ++k) {
            if (S[i][k] == S[i + 1][k]) K[i][i + 1][k] = true;
            else R[i][i + 1][k] = true;
        }
    }
    for (int l = 2; l <= n; ++l) {
        for (int i = 0; i < n - l; ++i) {
            int j = i + l;
            K[i][j] = K[i + 1][j] & K[i][j - 1];
            R[i][j] = R[i + 1][j] | R[i][j - 1];
        }
    }
}

std::vector<std::pair<int, int>>
merge_intervals(const std::vector<std::pair<int, int>> &a, const std::vector<std::pair<int, int>> &b) { // O(a+b)
    std::vector<std::pair<int, int>> result;
    auto itb = b.begin();
    for (auto ita = a.begin(); ita != a.end(); ++ita) {
        std::pair<int, int> temp = *ita;
        while (itb != b.end() && temp.first <= itb->first && itb->first <= temp.second) {
            temp.second = std::max(temp.second, itb->second);
            itb++;
        }
        result.push_back(temp);
    }
    for (; itb != b.end(); ++itb)
        result.push_back(*itb);
    return result;
}

void strie::built_C(int n, int m, const std::vector<std::string> &S) { // O(k*n^3)
    C.resize(n, std::vector<std::vector<std::vector<std::pair<int, int>>>>(n,
                                                                           std::vector<std::vector<std::pair<int, int>>>(
                                                                                   m)));
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n - 1; ++i) {
            if (S[i][j] == S[i + 1][j])
                C[i][i + 1][j].push_back({i, i + 1});
            else {
                C[i][i + 1][j].push_back({i, i});
                C[i][i + 1][j].push_back({i + 1, i + 1});
            }
        }
    }
    for (int k = 0; k < m; ++k) {
        for (int l = 2; l < n; ++l) {
            for (int i = 0; i < n - l; ++i) {
                int j = i + l;
                C[i][j][k] = merge_intervals(C[i][j - 1][k], C[i + 1][j][k]);
            }
        }
    }
}


int strie::count_bits(std::bitset<maxm> bs, int m) {
    int result = 0;
    for (int i = 0; i < m; ++i) result += bs[i];
    return result;
}

void strie::built_opt(int n, int m) {
    opt.resize(n, std::vector<int>(n));
    P.resize(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) opt[i][i] = 0;
    for (int l = 1; l < n; ++l) {
        for (int i = 0; i < n - l; ++i) {
            int j = i + l;
            std::vector<int> R_;
            for (int k = 0; k < m; ++k) if (R[i][j][k]) R_.push_back(k);
            opt[i][j] = 1e9;
            for (const auto &r: R_) {
                int temp_op = 0;
                for (const auto &p: C[i][j][r])
                    temp_op += opt[p.first][p.second] + count_bits(K[p.first][p.second], m) - count_bits(K[i][j], m);
                if (temp_op < opt[i][j]) {
                    P[i][j] = r;
                    opt[i][j] = temp_op;
                }
            }
        }
    }
}

strie::strie(int n, int m, const std::vector<std::string> &S) {
    built_KR(S, n, m);
    built_C(n, m, S);
    built_opt(n, m);
    int edges = count_bits(K[0][S.size() - 1], m) + opt[0][S.size() - 1];
    std::cout << edges << '\n';
}

std::ostream &operator<<(std::ostream &os, const strie &st) {
    return os;
}

int strie::get_size() {
    return trie.size();

}

//  Imprimir C
//    for (int r = 0; r < m; ++r) {
//        std::cout << "C" << r << '\n';
//        for (int i = 0; i < n; ++i) {
//            std::cout << "i: " << i << '\t';
//            for (int j = 0; j < n; ++j) {
//                std::cout << std::setw(30);
//                std::string c = "{";
//                for (const auto &e: C[i][j][r]) {
//                    c += "(" + std::to_string(e.first) + " " + std::to_string(e.second) + ")";
//                }
//                c += "}";
//                std::cout << c;
//            }
//            std::cout << '\n';
//        }
//    }


//int strie::opt_(int i, int j, int m) {
//    if (i == j) return 0;
//    std::vector<int> R_;
//    for (int k = 0; k < m; ++k)
//        if (R[i][j][k]) R_.push_back(k);
//    int result = 0;
//    for (const auto &r: R_) {
//        result = opt_()
//    }
//}
