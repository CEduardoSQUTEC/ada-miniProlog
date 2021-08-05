//
// Created by ceduardosq on 8/5/21.
//

#include "sptrie.h"

bool cmpMap(std::pair<char, int> &a, std::pair<char, int> &b) {
    return a.second < b.second;
}

std::map<char, int> sortMap(std::map<char, int> &m) {
    std::vector<std::pair<char, int>> v;

    for(auto& it : m) { v.push_back(it);}
    std::sort(v.begin(), v.end(), cmpMap);
    std::map<char, int> nm(v.begin(), v.end());

    return nm;
}

std::ostream &operator<<(std::ostream &os, sptrie &sp) {
    os << "Edges: " << sp.nodes << '\n';
    os << "s-trie:\n";
    return os;
}

