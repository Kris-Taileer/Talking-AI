#include "tokenisator.h"
#include "vocabulary.cpp"

int main() {
    sqlite3* db;
    sqlite3_open("mem.db", &db);
    unordered_map<string, unordered_map<string, int>> memory;

    if (sqlite3_open("mem.db", &db)) {
        cerr << "err opening db" << "\n";
        return 1;
    }
    init_db(db);
    load_mem(db, memory);
    cout << "ur message: ";
    string text;
    getline(cin, text);
    vector<string> tokens = tokenisator(text);
    train(db, tokens, memory);

    for (const auto& pair: memory) {
        cout << "Word: " << pair.first << " -> ";
        for (const auto& neighbor: pair.second) {
            cout << neighbor.first << "(" << neighbor.second << ") ";
        }
        cout << "\n";
    }

    sqlite3_close(db);

    return 0;
}
