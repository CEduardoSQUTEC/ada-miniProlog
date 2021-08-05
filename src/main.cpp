// #include <iostream>
// #include <vector>
// #include "strie.h"
// #include <fstream>
// #include <chrono>

// int main() {
//     std::string filename = "inMed";
//     // Open and read file
//     std::ifstream input_file("../input/" + filename + ".txt");
//     std::vector<std::string> S;
//     std::string input;
//     while (std::getline(input_file, input)) S.push_back(input);
//     input_file.close();
//     int n = S.size(), m = S[0].size();

//     // Recursive Trie
// //    std::cout << "<Recursive Trie>\n";
// //    std::chrono::time_point<std::chrono::system_clock> start_rec, end_rec;
// //    start_rec = std::chrono::system_clock::now();
// ////    strie strec(n, m, S, strie::recursive);
// //    end_rec = std::chrono::system_clock::now();
// //    std::chrono::duration<double> elapsed_seconds_rec = end_rec - start_rec;
// //    std::cout << "elapsed time: " << elapsed_seconds_rec.count() << "s\n";
// //    std::cout << strec << '\n';

//     // Memory Trie
//     std::cout << "<Memory Trie>\n";
//     std::chrono::time_point<std::chrono::system_clock> start_mem, end_mem;
//     start_mem = std::chrono::system_clock::now();
//     strie stmem(n, m, S, strie::memory);
//     end_mem = std::chrono::system_clock::now();
//     std::chrono::duration<double> elapsed_seconds_mem = end_mem - start_mem;
//     std::cout << "elapsed time: " << elapsed_seconds_mem.count() << "s\n";
// //    std::cout << stmem << '\n';

//     // DP Trie
//     std::cout << "<DP Trie>\n";
//     std::chrono::time_point<std::chrono::system_clock> start_dp, end_dp;
//     start_dp = std::chrono::system_clock::now();
//     strie stdp(n, m, S, strie::dp);
//     end_dp = std::chrono::system_clock::now();
//     std::chrono::duration<double> elapsed_seconds_dp = end_dp - start_dp;
//     std::cout << "elapsed time: " << elapsed_seconds_dp.count() << "s\n";
// //    std::cout << stdp << '\n';

//     // Save and close file
// //    std::ofstream output_file_rec("../output/" + filename + "-rec.txt");
// //    output_file_rec << strec.to_string();
// //    output_file_rec.close();

//     std::ofstream output_file_mem("../output/" + filename + "-mem.txt");
//     output_file_mem << stmem.to_string();
//     output_file_mem.close();

//     std::ofstream output_file_dp("../output/" + filename + "-dp.txt");
//     output_file_dp << stdp.to_string();
//     output_file_dp.close();

//     return 0;
// }

#include "sptrie.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::string filename = "inBig2";
    // Open and read file
    std::ifstream input_file("../input/" + filename + ".txt");
    std::vector<std::string> S;
    std::string input;
    while (std::getline(input_file, input)) S.push_back(input);
    input_file.close();
    int n = S.size(), m = S[0].size();

    std::cout << "<S-pTrie>\n";
    sptrie sp(S, n, m);
    std::chrono::time_point<std::chrono::system_clock> start_mem, end_mem;
    start_mem = std::chrono::system_clock::now();
    auto min = sp.greedyMinTrie();
    end_mem = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_mem = end_mem - start_mem;
    std::cout << "elapsed time: " << elapsed_seconds_mem.count() << "s\n";

    std::ofstream output_file_greedy("../output/" + filename + "-g.txt");
    output_file_greedy << min.printTrie();
    output_file_greedy.close();
    return 0;
}