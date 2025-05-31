#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, unordered_map<string, int>> memory;

void train(const vector<string>& tokens, unordered_map<string, unordered_map<string, int>>& memory, int win = 1) {
    for (int i = 0; i < tokens.size(); ++i) {
        string center = tokens[i];
        for (int j = max(0, i - win); j <= min((int)tokens.size() - 1, i + win); ++j) {
            if (j != i) {
                string neighbor = tokens[j];
                memory[center][neighbor]++;
            }
        }
    }
}
