#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, string> get_dict(string file_name) {
    unordered_map<string, string> eng_dict;
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "failed opening csv file" << "\n";
        return eng_dict;
    }

    string line, word, pos;
    string skip;

    getline(file, skip); //skip title i guess

    while (getline(file, line)) {
        stringstream ss(line);
        if (getline(ss, word, ',')&& getline(ss, pos, ',')) {
            eng_dict[word] = pos;
        }
    }

    return eng_dict;
}