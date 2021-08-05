//
// Created by ceduardosq on 8/5/21.
//

#include "sptrie.h"

std::ostream &operator<<(std::ostream &os, sptrie &sp) {
    os << "Edges: " << sp.nodes << '\n';
    os << "s-trie:\n";
    return os;
}
