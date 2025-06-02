#include "RESPONSE-ibility.cpp"

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
    string text;
    while (true) {
        cout << "ur message: ";
        getline(cin, text);
        if (text == "exit") break;
        vector<string> tokens = tokenisator(text);
        train(db, tokens, memory);

        if (!tokens.empty()) {
            string answer = res_gen(db, tokens.back());
            cout << "?: " << answer << "\n";
        }
    }

    sqlite3_close(db);

    return 0;
}
