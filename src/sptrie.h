//
// Created by ceduardosq on 8/5/21.
//

#ifndef SRC_SPTRIE_H
#define SRC_SPTRIE_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

bool cmpMap(std::pair<char, int>& a, std::pair<char,int>& b);

std::map<char, int> sortMap(std::map<char, int> &m);

struct sptrieNode {
    int id;
    std::map<int, sptrieNode> children;
    sptrieNode() {}
    sptrieNode(int i) {id = i;}
};

struct sptrie {
    std::vector<std::string> S;
    std::vector<int> p;
    int m = 0;
    int n = 0;
    int nodes = 0;
    std::vector<std::vector<char>> levels;
    std::map<char, int> diffs;
    sptrieNode root = sptrieNode(0);
    std::vector<sptrieNode> nodesList;

    int getIndex(char ch) {
        int num = ch;
        return num;
    }

    void insert(std::string string_) {
        auto pointer = this->root;
        int stringLength = string_.length();

        for (int character = 0; character < stringLength; character++) {
            auto index = getIndex(string_[character]);
            if(pointer.children.find(index) == pointer.children.end()) {
                sptrieNode node = sptrieNode(this->nodes);
                pointer.children[index] = node;
                this->nodes++;
                this->nodesList.push_back(pointer.children[index]);
            }
            pointer = pointer.children[index];
        }
    }

    void setLevels(int m, int n) {
        this->m = m;
        this->n = n;
        for (int level = 0; level < m; level++) {
            std::vector<char> elems;
            for (int j = 0; j < n; j++) {
                elems.push_back(S[j][level]);
            }
            levels.push_back(elems);
        }
    }

    std::string printTrie() {
        char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
	    std::vector<char> alphabet( alpha, alpha+sizeof(alpha)-1 );
        std::string result = "";
        for (auto i : this->nodesList) {
            result += i.id + " [";
            std::vector<std::pair<char, int>> temp;
            for (int j = 0; j < alphabet.size(); j++) {
                if (i.children.find(j) != i.children.end()) {
                    temp.push_back(std::make_pair('a', i.children[j].id));
                }
            }
            std::sort(temp.begin(), temp.end(), cmpMap);
            for (auto it = temp.begin(); it != temp.end(); ++it) {
                if (it != temp.begin()) result += ", ";
            result += "(\'" + std::string(1, it->first) + "\'" + ", " + std::to_string(it->second) + ")";
        }
        result += "]\n";
        }
        return result;
    }

    void calculateDiffsPerLevel(std::vector<char> level, int i) {
        std::unordered_set<char> already;
        for (int j; j < level.size(); j++) {
            already.insert(level[j]);
        }
        this->diffs[i] = already.size();
    }

    void calculateDiffs() {
        int i = 0;
        for (auto level : levels) {
            diffs[i] = 0;
            calculateDiffsPerLevel(level, i);
            i++;
        }
    }

    sptrie greedyMinTrie() {
        this->calculateDiffs();
        std::map<char, int> d = sortMap(diffs);
        sptrie minSptrie;
        for (auto it = d.begin(); it != d.end(); it++) {
            minSptrie.p.push_back(it->first);
        }
        for (auto string_ : S) {
            auto greedyString = "";
            for (auto it = d.begin(); it != d.end(); it++) {
                greedyString += string_[it->first];
            }
            minSptrie.S.push_back(greedyString);
        }
        for(auto string_ : this->S) {
            minSptrie.insert(string_);
        }
        minSptrie.setLevels(m, n);
        return minSptrie;
    }

    sptrie() {}

    sptrie(const std::vector<std::string> &S, int n, int m) {
        this->S = S;

        std::vector<int> pTemp(0, m);
        this->p = pTemp;
        std::cout<<"P: (";
        for (auto x : p) {std::cout<<x<<", ";}
        std::cout<<")\n";
        
        for (auto string_: S) {insert(string_);}
        setLevels(m, n);
        std::cout<<"S-ptrie created with "<<nodes<<" nodes\n";
    }
};


#endif //SRC_SPTRIE_H
