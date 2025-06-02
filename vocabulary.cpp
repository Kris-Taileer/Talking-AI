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
        CREATE TABLE vocabulary (
            context TEXT NOT NULL,
            next_word TEXT NOT NULL,
            weight INTEGER NOT NULL,
            n INTEGER NOT NULL,
            PRIMARY KEY (context, next_word, n)
        );

    )";
    sqlite3_exec(db, sql, 0, 0, nullptr);
}

void train_ngram(sqlite3* db, const vector<string>& tokens, int n) {
    for (int i = 0; i + n <= tokens.size(); ++i) {
        string context;
        for (int j = 0; j < n - 1; ++j) {
            if (j > 0) context += " ";
            context += tokens[i + j];
        }
        string next_word = tokens[i + n - 1];

        sqlite3_stmt* tmp;
        const char* sql = "INSERT INTO vocabulary (context, next_word, weight, n) "
                          "VALUES (?, ?, 1, ?) "
                          "ON CONFLICT(context, next_word, n) DO UPDATE SET weight = weight + 1;";


        if (sqlite3_prepare_v2(db, sql, -1, &tmp, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(tmp, 1, context.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(tmp, 2, next_word.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(tmp, 3, n);
            sqlite3_step(tmp);
        }
        sqlite3_finalize(tmp);
    }
}

string get_next(sqlite3* db, const string& context, int n) {
    sqlite3_stmt* tmp;
    string next;

    const char* sql = "SELECT next_word FROM vocabulary "
                      "WHERE context = ? AND n = ? "
                      "ORDER BY weight DESC LIMIT 1;";


    if (sqlite3_prepare_v2(db, sql, -1, &tmp, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(tmp, 1, context.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(tmp, 2, n);
        if (sqlite3_step(tmp) == SQLITE_ROW) {
            const char* word = (const char*)sqlite3_column_text(tmp, 0);
            next = word;
        }
    }

    sqlite3_finalize(tmp);

    return next;
}

string generate_response(sqlite3* db, const string& text) {
    vector<string> tokens = tokenisator(text);
    int max_n = 4;
    string res;
    string context;

    for (int n = max_n; n >= 2; --n) {
        if (tokens.size() >= n - 1) {
            context.clear();
            for (int i = tokens.size() - (n - 1); i < tokens.size(); ++i) {
                if (!context.empty()) context += " ";
                context += tokens[i];
            }
            res = context;
            for (int i = 0; i < 15; ++i) {
                string next = get_next(db, context, n);
                if (next.empty()) break;
                res += " " + next;

                vector<string> context_tokens_fuck_naming = tokenisator(context);
                context_tokens_fuck_naming.push_back(next);
                if (context_tokens_fuck_naming.size() > n - 1) context_tokens_fuck_naming.erase(context_tokens_fuck_naming.begin());

                context.clear();
                for (const auto& word: context_tokens_fuck_naming) {
                    if (!context.empty()) context += " ";
                    context += word;
                }
            }

            if (res != context) break;
        }
    }

    return res;
}
