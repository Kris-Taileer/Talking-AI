#include <iostream>
#include "tokenisator.h"
#include "vocabulary.cpp"

int main() {
    sqlite3* db;
    sqlite3_open("mem1.db", &db);
    unordered_map<string, unordered_map<string, int>> memory;

    if (sqlite3_open("mem1.db", &db)) {
        cerr << "err opening db" << "\n";
        return 1;
    }
    init_db(db);
    string text;
    while (true) {
        cout << "ur message: ";
        getline(cin, text);
        if (text == "exit") break;
        vector<string> tokens = tokenisator(text);

        for (int n = 2; n <= 4; ++n) {
            train_ngram(db, tokens, n);
        }

        string answer = generate_response(db, text);
        cout << "?: " << answer << "\n";
    }

    sqlite3_close(db);

    return 0;
}
