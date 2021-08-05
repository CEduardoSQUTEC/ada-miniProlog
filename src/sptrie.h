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
#include <utility>
#include <functional>

bool cmpMap(std::pair<char, int>& a, std::pair<char,int>& b);

std::vector<std::pair<char, int>> sortMap(std::map<char, int> &m);

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
    sptrieNode* root = new sptrieNode(0);
    std::vector<sptrieNode> nodesList;

    int getIndex(char ch) {
        int num = ch;
        return num;
    }

    void insert(std::string string_) {
        sptrieNode* pointer = this->root;
        int stringLength = string_.length();

        for (int character = 0; character < stringLength; character++) {
            auto index = getIndex(string_[character]);
            if(pointer->children.find(index) == pointer->children.end()) {
                sptrieNode node = sptrieNode(this->nodes);
                pointer->children[index] = node;
                this->nodes++;
                this->nodesList.push_back(pointer->children[index]);
            }
            pointer = &(pointer->children[index]);
        }
    }

    void setLevels(int m, int n) {
        this->m = m;
        this->n = n;
        for (int level = 0; level < m; level++) {
            std::vector<char> elems;
            for (int i = 0; i < n; i++) {
                elems.push_back(S[i][level]);
            }
            levels.push_back(elems);
        }
    }

    std::string printTrie() {
        char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
	    std::vector<char> alphabet( alpha, alpha+sizeof(alpha)-1 );
        std::string result = "";
        for (auto i : nodesList) {
            result += std::to_string(i.id) + " [";
            std::vector<std::pair<char, int>> temp;
            for (int j = 0; j < alphabet.size(); j++) {
                if (i.children.find((char)j) != i.children.end()) {
                    std::pair<char, int> a = std::make_pair(('a' + j), i.children[j].id);
                    temp.push_back(a);
                }
            }
            std::sort(temp.begin(), temp.end(), cmpMap);
            for (auto it = temp.begin(); it != temp.end(); ++it) {
                if (it != temp.begin()) result += ", ";
            result += "(\'" + std::to_string(it->first) + "\'" + ", " + std::to_string(it->second) + ")";
        }
        result += "]\n";
        }
        return result;
    }

    void calculateDiffsPerLevel(std::vector<char> level, int i) {
        std::unordered_set<char> already;
        for (int j = 0; j < level.size(); j++) {
            already.insert(level[j]);
        }
        diffs[i] = already.size();
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
        std::cout << "\n<Min S-pTrie>\n";
        this->calculateDiffs();
        std::vector<std::pair<char, int>> d = sortMap(diffs);
        sptrie minSptrie;
        for (auto it = d.begin(); it != d.end(); it++) {
            minSptrie.p.push_back(it->first);
        }
        for (auto string_ : S) {
            std::string greedyString = "";
            for (auto x : minSptrie.p) {
                greedyString += string_[x];
            }
            minSptrie.S.push_back(greedyString);
        }

        nodesList.push_back(*root);
        for(auto string_ : minSptrie.S) {

            minSptrie.insert(string_);
        }
        minSptrie.setLevels(m, n);
        std::cout<<"Min S-ptrie created with "<<minSptrie.nodes<<" nodes\n";
        return minSptrie;
    }

    sptrie() {}

    sptrie(const std::vector<std::string> &S, int n, int m) {
        this->S = S;

        std::vector<int> pTemp(m);
        for (int i = 0; i < m; i++) {pTemp[i] = i;}
        p = pTemp;
        
        for (auto string_: S) {
            insert(string_);
        }

        setLevels(m, n);
        nodesList.push_back(*root);
        std::cout<<"S-ptrie created with "<<nodes<<" edges\n";
    }
};


#endif //SRC_SPTRIE_H
