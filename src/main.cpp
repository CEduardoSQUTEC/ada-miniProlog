#include <iostream>
#include <vector>
#include "strie.h"
#include <fstream>
#include <chrono>

int main() {
    std::string filename = "basicTest02";
    // Open and read file
    std::ifstream input_file("../input/" + filename + ".txt");
    std::vector<std::string> S;
    std::string input;
    while (std::getline(input_file, input)) S.push_back(input);
    input_file.close();
    int n = S.size(), m = S[0].size();

    // Recursive Trie
    std::cout << "<Recursive Trie>\n";
    std::chrono::time_point<std::chrono::system_clock> start_rec, end_rec;
    start_rec = std::chrono::system_clock::now();
    strie strec(n, m, S, strie::recursive);
    end_rec = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_rec = end_rec - start_rec;
    std::cout << "elapsed time: " << elapsed_seconds_rec.count() << "s\n";
    std::cout << strec << '\n';

    // DP Trie
    std::cout << "<DP Trie>\n";
    std::chrono::time_point<std::chrono::system_clock> start_dp, end_dp;
    start_dp = std::chrono::system_clock::now();
    strie stdp(n, m, S, strie::dp);
    end_dp = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_dp = end_dp - start_dp;
    std::cout << "elapsed time: " << elapsed_seconds_dp.count() << "s\n";
    std::cout << stdp << '\n';

    // Save and close file
    std::ofstream output_file("../output/" + filename + ".trie");
    output_file << stdp.to_string();
    output_file.close();
    return 0;
}
