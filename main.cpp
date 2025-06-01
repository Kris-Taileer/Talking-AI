#include "tokenisator.h"
#include "vocabulary.h"

int main() {
    string path = "db.txt";
    ofstream fout(path);

    if (!fout.is_open()) {
        cerr << "err opening file" << "\n";
        return 1;
    }

    cout << "ur message: ";
    string text;
    getline(cin, text);
    vector<string> tokens = tokenisator(text);
    train(tokens, memory);

    for (const string& token : tokens) {
        fout << token << "\n";
        cout << token << "\n";
    }

    for (const auto& pair: memory) {
        cout << "Word: " << pair.first << " -> ";
    for (const auto& neighbor: pair.second) {
        cout << neighbor.first << "(" << neighbor.second << ") ";
    }
    cout << "\n";
}

    fout.close();

    cout << "file path: " << path << "\n";
    return 0;
}
