#include <iostream>
#include <vector>
#include "strie.h"
#include <fstream>
#include <chrono>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;


int main() {
    std::string filename = "inMed.txt";
    std::ifstream input_file("input/" + filename);
    std::vector<std::string> S;
    std::string input;
    while (std::getline(input_file, input)) S.push_back(input);
    input_file.close();
    int n = S.size(), m = S[0].size();

    auto t0 = Time::now();
    strie st(n, m, S);
    auto t1 = Time::now();
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << fs.count() << "s\n";

    std::ofstream output_file("output/" + filename);
    output_file << st.to_string();
    output_file.close();
    return 0;
}
