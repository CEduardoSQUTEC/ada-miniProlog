#include "sptrie.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::string filename = "inMed";
    // Open and read file
    std::ifstream input_file("../input/" + filename + ".txt");
    std::vector<std::string> S;
    std::string input;
    while (std::getline(input_file, input)) S.push_back(input);
    input_file.close();
    int n = S.size(), m = S[0].size();    
    sptrie sp;

    std::cout << "<S-pTrie>\n";
    std::chrono::time_point<std::chrono::system_clock> start_mem, end_mem;
    start_mem = std::chrono::system_clock::now();
    auto min = sp.greedyMinTrie();
    end_mem = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_mem = end_mem - start_mem;
    std::cout << "elapsed time: " << elapsed_seconds_mem.count() << "s\n";

    std::ofstream output_file_greedy("../output/" + filename + "-dp.txt");
    output_file_greedy << sp.printTrie();
    output_file_greedy.close();
    return 0;
}