#include <iostream>
#include "tokenisator.h"
#include "vocabulary.cpp"

using namespace std;

string res_gen(sqlite3* db, const string& start_w, int max_len = 10) {
    string current = start_w;
    string res = start_w;

    for (int i = 0; i < max_len; ++i) {
        string next;
        int max_weight = -1; //sql max num
        string sql = "SELECT neighbor, weight FROM memory WHERE word = ? ORDER BY weight DESC LIMIT 1;"; //getting first word with max weight with limit 1;

        sqlite3_stmt* tmp;
        sqlite3_bind_text(tmp, 1, current.c_str(), -1, SQLITE_TRANSIENT); //bind curr word to ? in sql request
        if (sqlite3_step(tmp) == SQLITE_ROW) { //if we have what to read, read it and give me its naighbor
            const char* neighbor = (const char*)sqlite3_column_text(tmp, 0);
            next = neighbor;
        }

        sqlite3_finalize(tmp); //no mem leak

        if (next.empty()) break;
        res += " " + next;
        current = next;
    }

    return res;

}