#include "compiler.h"
#include "strie.h"

compiler::compiler() = default;

std::map<int, std::map<char, int>> compiler::parse_trie() const {
    std::string path = std::string("output/") + (compile_opt ? "opt.txt": "greedy.txt");
    std::map<int, std::map<char, int>> trie_obj;
    std::ifstream trie_code(path);
    std::string buffer;
    while(std::getline(trie_code, buffer)){
        std::string node_index;
        auto line = buffer.begin();
        while (*line != ' ') node_index += *(line)++;
        std::map<char, int> edges;
        if(*(++line) != ';') {
            while(line != buffer.end()){
                char label = *line;
                std::string adj_index;
                while (*(++line) != ';') adj_index += *line;
                edges.insert(std::make_pair(label, std::stoi(adj_index)));
                line++;
            }
        }
        trie_obj.insert(std::make_pair(std::stoi(node_index), edges));
    }
    return trie_obj;
}

std::unordered_map<std::string, std::map<int, std::map<char, int>>> compiler::parse_tries() {
    std::ifstream tries_file("tries.txt");
    std::string buffer, actual_trie, s_stream;
    std::vector<std::string> trie_chars;
    std::unordered_map<std::string,std::map<int, std::map<char, int>>> queries;
    while(std::getline(tries_file, buffer)) {
        auto line = buffer.begin();
        std::string trie_id;
        while(*line != ' ') trie_id += *(line++);
        if(!actual_trie.empty()) {
            if(actual_trie != trie_id){
                strie trie(trie_chars.size(), s_stream.size(), trie_chars, strie::dp);
                trie.save_trie(compile_opt);
                queries.insert(std::make_pair(actual_trie, parse_trie()));
                actual_trie = trie_id;
                trie_chars.clear();
            }
        }
        else actual_trie = trie_id;
        s_stream = "";
        while(++line != buffer.end()) s_stream += *line;
        trie_chars.push_back(s_stream);
    }
    if(!trie_chars.empty()){
        strie trie(trie_chars.size(), s_stream.size(), trie_chars, strie::dp);
        trie.save_trie(compile_opt);
        queries.insert(std::make_pair(actual_trie, parse_trie()));
    }
    return queries;
}

void compiler::exec_queries() {
    auto tries = parse_tries();
    std::ifstream query_file("queries.txt");
    std::string buffer;
    while(std::getline(query_file, buffer)){
        auto line = buffer.begin();
        std::string trie_id, s_stream;
        while(*line != ' ') trie_id += *(line++);
        while(++line != buffer.end()) s_stream += *line;
        auto res = perform_query(tries[trie_id], s_stream);
        std::cout<<trie_id<<": X = ";
        if(!res.empty()){
            auto r = res.begin();
            while(r != res.end()){
                std::cout<<*r;
                if(++r != res.end()) std::cout<<", ";
                else std::cout<<std::endl;
            }
        }
        else std::cout<<"- \n";
    }
}

std::vector<char> compiler::perform_query(std::map<int, std::map<char, int>> trie, std::string query) {
    auto node = trie[0];
    std::vector<char> res;
    auto next = query.begin();
    while(next != query.end()){
        if(*next == 'X') break;
        else{
            auto new_node = node.find(*next);
            if(new_node == node.end()) return {};
            node = trie[node[*(next++)]];
        }
    }
    return multipath_search(node, trie, ++next, query.end());
}

std::vector<char>
compiler::multipath_search(const std::map<char, int> &valid_nodes, std::map<int, std::map<char, int>> trie,
                           std::string::iterator next, std::string::iterator stop) {
    std::map<char, std::map<char, int>> X_nodes;
    for(auto it: valid_nodes) X_nodes[it.first] = trie[it.second];
    while(next != stop && !X_nodes.empty()){
        for(auto it: X_nodes){
            if(it.second.find(*next) == it.second.end()){
                X_nodes.erase(it.first);
                if(X_nodes.empty()) break;
            }
            else X_nodes[it.first] = trie[it.second[*(next++)]];
        }
    }
    std::vector<char> res;
    for(const auto& it: X_nodes) res.push_back(it.first);
    return res;
}

void compiler::display_menu() {
    std::cout<<"====*====*== MINICOMPILADOR DE MINIPROLOG ==*====*====\n";
    std::cout<<"* 1. Setear modo de compilación heurística.\n";
    std::cout<<"* 2. Setear modo de compilación óptima.\n";
    std::cout<<"* 3. Compilar archivo.\n";
    std::cout<<"* 4. Terminar el programa.\n";
    std::cout<<"* OPC: Selecciona una opción.\n";
    do {
        size_t option = 0;
        std::cin>>option;
        switch(option){
            case 1: {
                compile_opt = false;
                std::cout<<"\t*== MODE: Se compilara con el algoritmo voraz.\n";
                break;
            }
            case 2: {
                compile_opt = false;
                std::cout<<"\t*== MODE: Se compilara con el algoritmo óptimo.\n";
                break;
            }
            case 3: {
                std::cout<<"\t*== COMP: Resultados del archivo de consultas:\n";
                exec_queries();
            }
            case 4:
            default: return;
        }
    } while(true);
}