#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sqlite3.h>
//this is the most complicated shit i have ever done for last 1/2 year....still sql is more like a blackbox for me, god bless internet guides

using namespace std;

void init_db(sqlite3* db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS memory (
            word TEXT NOT NULL,
            neighbor TEXT NOT NULL,
            weight INTEGER NOT NULL,
            PRIMARY KEY (word, neighbor)
        );

    )";
    sqlite3_exec(db, sql, 0, 0, nullptr);
}

void insert_token(sqlite3* db, const string& word, const string& neighbor, int weight) {
    const char* sql = "INSERT OR REPLACE INTO memory (word, neighbor, weight) VALUES (?, ?, ?);"; //FUCK SQL
    sqlite3_stmt* tmp;

    if (sqlite3_prepare_v2(db, sql, -1, &tmp, nullptr) != SQLITE_OK) {
        cerr << "err in request preparing" << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(tmp, 1, word.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(tmp, 2, neighbor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(tmp, 3, weight);

    if (sqlite3_step(tmp) != SQLITE_DONE) {
        cerr << "request achiving err " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(tmp);
}

void load_mem(sqlite3* db, unordered_map<string, unordered_map<string, int>>& memory) { //STILL FUCK IT
    const char* sql = "SELECT word, neighbor, weight FROM memory;";
    sqlite3_stmt* tmp;

    if (sqlite3_prepare_v2(db, sql, -1, &tmp, nullptr) != SQLITE_OK) {
        cerr << "SELECT request err " << sqlite3_errmsg(db) << "\n";
        return;
    }

    while (sqlite3_step(tmp) == SQLITE_ROW) {
        string word = reinterpret_cast<const char*>(sqlite3_column_text(tmp, 0));
        string neighbor = reinterpret_cast<const char*>(sqlite3_column_text(tmp, 1));
        int weight = sqlite3_column_int(tmp, 2);

        memory[word][neighbor] = weight;
    }

    sqlite3_finalize(tmp);
}

void train(sqlite3* db, vector<string>& tokens, unordered_map<string, unordered_map<string, int>>& memory) { // as expected, at last i should have to rewrite train function for sql db
    for (int i = 0; i + 1 < tokens.size(); ++i) {
        string w1 = tokens[i];
        string w2 = tokens[i + 1];
        memory[w1][w2]++;
        insert_token(db, w1, w2, memory[w1][w2]);

    }
}
