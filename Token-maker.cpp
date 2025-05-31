#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector <string> tokenisator(const string& str) {
    vector <string> list;
    string word;
    for (int i = 0; i < str.size(); ++i) {
        char ch = str[i];
        ch = tolower(ch);
        if (isalnum(ch)) {
            word += ch;
        } else {
            if (!word.empty()) {
                list.push_back(word);
                word.clear();
            }
        }
    }

    if (!word.empty()) {
        list.push_back(word);
        word.clear();
    }

    return list;
}

