#pragma once

#include <sqlite3.h>
#include <string>
#include <iostream>

class DBLite
{
public:
    DBLite()
    {
        res = sqlite3_open("/Users/bodya/Desktop/С++/AdvancedC++/Sratoslav/Sratoslav/DataBase/DBBot.db", &db);
        checkDBErrors();
    }
    void createUserTable()
    {
        sql = "CREATE TABLE IF NOT EXISTS USERS("
              "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "USER_ID             INTEGER NOT NULL, "
              "USERNAME            TEXT    NOT NULL);";

        res = sqlite3_exec(db, sql.c_str(), callback, 0, &err_msg);
    }
    void createPhraseTable()
    {
        sql = "CREATE TABLE IF NOT EXISTS PHRASES("
              "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "USER_ID             INTEGER NOT NULL, "
              "PHRASES             TEXT    NOT NULL);";

        res = sqlite3_exec(db, sql.c_str(), callback, 0, &err_msg);
    }
    void insertData(const std::string &table, int user_id, const std::string &str)
    {
        if(table == "USERS")
        {
            char *query = nullptr;

            asprintf(&query, "INSERT INTO USERS ('USER_ID', 'USERNAME') VALUES  ('%d', '%s');", user_id, str.c_str());

            sqlite3_prepare(db, query, strlen(query), &stmt, nullptr);

            res = sqlite3_step(stmt);

            sqlite3_finalize(stmt);

            free(query);
        }
        else if(table == "PHRASES")
        {
            char *query = nullptr;

            asprintf(&query, "INSERT INTO PHRASES ('USER_ID', 'PHRASES') VALUES  ('%d', '%s');", user_id, str.c_str());

            sqlite3_prepare(db, query, strlen(query), &stmt, nullptr);

            res = sqlite3_step(stmt);

            sqlite3_finalize(stmt);

            free(query);
        }
    }
    void closeDB()
    {
        sqlite3_close(db);
    }
private:
    sqlite3 *db;
    sqlite3_stmt *stmt;

    int res;

    char *err_msg;
    std::string sql;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        for(int i = 0; i < argc; i++)
        {
            std::cout << azColName[i] << ": " << argv[i] << std::endl;
        }
        std::cout << std::endl;

        return 0;
    }

    void checkDBErrors()
    {
        if( res )
        {
            std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
            closeDB();
        }
    }
};


