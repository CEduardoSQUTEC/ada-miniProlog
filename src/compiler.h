#ifndef SRC_COMPILER_H
#define SRC_COMPILER_H
#include "lib.h"

class compiler {
    bool compile_opt;
    [[nodiscard]] std::map<int, std::map<char, int>> parse_trie() const;
    std::unordered_map<std::string,std::map<int, std::map<char, int>>> parse_tries();
    std::vector<char> perform_query(std::map<int, std::map<char, int>> trie,
                                    std::string query);
    static std::vector<char> multipath_search(const std::map<char, int>& valid_nodes,
                                       std::map<int, std::map<char, int>> trie,
                                       std::string::iterator next, std::string::iterator stop);
    void exec_queries();
public:
    compiler();
    void display_menu();
};


#endif //SRC_COMPILER_H
