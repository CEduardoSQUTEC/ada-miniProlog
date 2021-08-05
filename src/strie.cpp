#include "strie.h"
#include <iostream>

int count_bits(std::bitset <maxm> bs, int m) {
    int result = 0;
    for (int i = 0; i < m; ++i) result += bs[i];
    return result;
}

std::vector <std::pair<int, int>>
merge_intervals(const std::vector <std::pair<int, int>> &a, const std::vector <std::pair<int, int>> &b) { // O(a+b)
    std::vector <std::pair<int, int>> result;
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


void strie::built_KR(const std::vector <std::string> &S, int n, int m) { // O(n^2m)
    K.resize(n, std::vector < std::bitset < maxm >> (n));
    R.resize(n, std::vector < std::bitset < maxm >> (n));
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

void strie::built_C(int n, int m, const std::vector <std::string> &S) { // O(k*n*m)
    C.resize(n, std::vector < std::vector < std::vector < std::pair < int, int >> >> (n,
            std::vector < std::vector < std::pair < int, int>>>(
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


int strie::opt_memory(int i, int j, const std::vector <std::string> &S) {
    if (i == j) return 0;
    if (opt[i][j] != -1) return opt[i][j];

    int m = S[i].size();
    opt[i][j] = 1e9;

    if (!KRB[i][j]) {
        auto tempK = K_recursive(i, j, S);
        K[i][j] = tempK;
        R[i][j] = tempK.flip();
        KRB[i][j] = true;
    }

    std::vector<int> R_;
    for (int u = 0; u < m; ++u) if (R[i][j][u]) R_.push_back(u);
    for (const auto &r: R_) {
        int temp_op = 0;
        if (!CB[i][j][r]) {
            C[i][j][r] = C_recursive(i, j, r, S);
            CB[i][j][r] = true;
        }
        for (const auto &p : C[i][j][r]) {
            if (!KRB[p.first][p.second]) {
                auto tempK = K_recursive(p.first, p.second, S);
                K[p.first][p.second] = tempK;
                R[p.first][p.second] = tempK.flip();
                KRB[p.first][p.second] = true;
            }
            temp_op += opt_memory(p.first, p.second, S) + count_bits(K[p.first][p.second], m) - count_bits(K[i][j], m);
        }
        if (temp_op < opt[i][j]) {
            P[i][j] = r;
            opt[i][j] = temp_op;
        }
    }
    return opt[i][j];
}


std::bitset <maxm> strie::K_recursive(int i, int j, const std::vector <std::string> &S) {
    int m = S[i].size();
    std::bitset <maxm> result((1LL << (m)) - 1);
    if (i == j) return result;
    for (int k = 0; k < m; ++k) {
        bool pos = true;
        for (int u = i; u < j; u++)
            if (S[u][k] != S[u + 1][k]) pos = false;
        result[k] = pos;
    }
    return result;
}

std::vector <std::pair<int, int>> strie::C_recursive(int i, int j, int r, const std::vector <std::string> &S) {
    std::vector <std::pair<int, int>> result;
    int start = i;
    for (int k = i; k < j; ++k) {
        if (S[k][r] != S[k + 1][r]) {
            result.push_back({start, k});
            start = k + 1;
        }
    }
    result.push_back({start, j});
    return result;
}

int strie::opt_recursive(int i, int j, const std::vector <std::string> &S) {
    int result = 1e9;
    int m = S[i].size();
    if (i == j) return 0;
    auto k_ = K_recursive(i, j, S);
    k_.flip();
    std::vector<int> R_;
    for (int u = 0; u < m; ++u) if (k_[u]) R_.push_back(u);
    for (const auto &r: R_) {
        int temp_op = 0;
        auto c = C_recursive(i, j, r, S);
        for (const auto &p: c)
            temp_op += opt_recursive(p.first, p.second, S) + count_bits(K_recursive(p.first, p.second, S), m) -
                       count_bits(K_recursive(i, j, S), m);
        if (temp_op < result) {
            P[i][j] = r;
            result = temp_op;
        }
    }
    return result;
}


std::bitset <maxm> strie::get_K(int i, int j, const std::vector <std::string> &S) {
    std::bitset <maxm> result{};
    switch (builder_) {
        case recursive: {
            result = K_recursive(i, j, S);
            break;
        }
        case memory:
        case dp: {
            result = K[i][j];
            break;
        }
    }
    return result;
}

std::vector <std::pair<int, int>> strie::get_C(int i, int j, int r, const std::vector <std::string> &S) {
    std::vector <std::pair<int, int>> result{};
    switch (builder_) {
        case recursive: {
            result = C_recursive(i, j, r, S);
            break;
        }
        case memory:
        case dp: {
            result = C[i][j][r];
            break;
        }
    }
    return result;
}

int strie::built_trie(int i, int j, int n, int m, const std::vector <std::string> &S, std::bitset <maxm> I) {
    // Save the current state
    int current_state = trie.size();
    // K_ = K[i,j] & I
    // - Before: auto temp_set = K[i][j] & I;
    auto temp_set = get_K(i, j, S) & I;
    I = I & (~temp_set);
    std::vector<int> K_;
    for (int k = 0; k < m; ++k) if (temp_set[k]) K_.push_back(k);
    // Set all the common states
    for (const auto &e: K_)
        trie.push_back(std::unordered_map<char, int>() = {{key_position, e}});
    // Set the edges between them
    for (int k = current_state; k < (int) trie.size() - 1; ++k) {
        char edge = S[i][trie[k][key_position]];
        trie[k][edge] = k + 1;
    }
    if (i == j) {
        trie.push_back(std::unordered_map<char, int>() = {{key_position, -1}});
        if (current_state < (int) trie.size() - 1) {
            char edge = S[i][trie[(int) trie.size() - 2][key_position]];
            trie[(int) trie.size() - 2][edge] = (int) trie.size() - 1;
        }
    } else {
        // C[i, j, r] Built the rest of the tree
        int r = P[i][j];
        trie.push_back(std::unordered_map<char, int>() = {{key_position, r}});
        if (current_state < (int) trie.size() - 1) {
            char edge = S[i][trie[(int) trie.size() - 2][key_position]];
            trie[(int) trie.size() - 2][edge] = (int) trie.size() - 1;
        }
        I[r] = false;
        int last_state = (int) trie.size() - 1;
        //      - Before: for (const auto &p: C[i][j][r]) {
        for (const auto &p: get_C(i, j, r, S)) {
            char edge = S[p.first][r];
            trie[last_state][edge] = built_trie(p.first, p.second, n, m, S, I);
        }
    }
    return current_state;
}

strie::strie(int n, int m, const std::vector <std::string> &S, builder b) {
    builder_ = b;
    switch (b) {
        case recursive: {
            P.resize(n, std::vector<int>(n));
            opt_recursive(0, n - 1, S);
            break;
        }
        case memory: {
            P.resize(n, std::vector<int>(n));
            opt.resize(n, std::vector<int>(n, -1));
            C.resize(n, std::vector < std::vector < std::vector < std::pair < int, int >> >> (n,
                    std::vector < std::vector < std::pair < int, int>>>(
                    m)));
            K.resize(n, std::vector < std::bitset < maxm >> (n));
            R.resize(n, std::vector < std::bitset < maxm >> (n));
            KRB.resize(n, std::vector<bool>(n, false));
            CB.resize(n, std::vector < std::vector < bool >> (n, std::vector<bool>(m, false)));
            opt_memory(0, n - 1, S);
            break;
        }
        case dp: {
            built_KR(S, n, m);
            built_C(n, m, S);
            built_opt(n, m);
            break;
        }
        default: {
            std::cerr << "Invalid build" << '\n';
        }
    }
    built_trie(0, S.size() - 1, n, m, S, std::bitset<maxm>((1LL << (m)) - 1));
}

int strie::get_size() {
    return trie.size();
}

void strie::print(int node, char edge = '*', int depth = 0) {
    for (int i = 0; i < depth - 1; ++i)
        std::cout << "|   ";
    if (depth > 0)
        std::cout << "|-" << edge << "-";
    int state = trie[node][key_position];
    if (state == -1) std::cout << 'o' << '\n';
    else std::cout << state << '\n';
    for (auto &child: trie[node]) {
        if (child.first == key_position) continue;
        print(child.second, child.first, depth + 1);
    }
}

std::string strie::to_string() {
    std::string result = "";
    for (int i = 0; i < trie.size(); ++i) {
        result += std::to_string(i) + " ";
        std::vector <std::pair<char, int>> a;
        for (const auto &e:trie[i]) {
            if (e.first == '$') continue;
            a.push_back({e.first, e.second});
        }
        for (auto it = a.rbegin(); it != a.rend(); ++it) {
            if (it != a.rbegin()) result += ";";
            result += std::string(1, it->first) + std::to_string(it->second);
        }
        result += ";\n";
    }
    return result;
}

void strie::save_trie(bool is_opt) {
    std::ofstream output_file(std::string("output/") + (is_opt ? "opt.txt" : "greedy.txt"));
    output_file << to_string();
    output_file.close();
}

std::ostream &operator<<(std::ostream &os, strie & st) {
    os << "Edges: " << st.get_size() - 1 << '\n';
    os << "s-trie:\n";
    st.print(0);
    return os;
}

