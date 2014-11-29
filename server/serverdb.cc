
#include <assert.h>
#include <stdint.h>

#include <unistd.h>

#include <set>
#include <string>
#include <iostream>
#include <memory>

#include "sqlite3.h"

#include "sqlstmt.h"
#include "serverdb.h"

using namespace std;

int
dirDepth(const string &path)
{
    int depth = 0;

    for (auto it = path.begin(); it != path.end(); it++) {
        if ((*it) == '/')
            depth++;
    }

    return depth;
}

ServerDB::ServerDB(const char *file)
{
    open(file);
}

ServerDB::~ServerDB()
{
    close();
}

bool
ServerDB::hasKey(const string &path)
{
    SQLStmt s(db, "SELECT key, value FROM kvpair WHERE key = \"%s\"",
            path.c_str());

    return s.step().row();
}

string
ServerDB::get(const string &path)
{
    SQLStmt s(db, "SELECT value FROM kvpair WHERE key = \"%s\"",
            path.c_str());

    s.step();
    if (!s.row()) {
        cout << "No Key: " << path << endl;
        throw runtime_error("Key not present");
    }

    return s.str(0);
}

bool
ServerDB::set(const string &path, const string &val)
{
    bool update = hasKey(path);
    sqlexec("BEGIN;");
    if (update) {
        sqlexec("UPDATE kvpair SET value = \"%s\" WHERE key = \"%s\";",
            val.c_str(), path.c_str());
    } else {
        int depth = dirDepth(path);
        sqlexec("INSERT INTO kvpair (key, value, depth) VALUES (\"%s\", \"%s\", %d);",
            path.c_str(), val.c_str(), depth);
    }
    sqlexec("COMMIT;");
    return update;
}

void
ServerDB::remove(const string &path)
{
    if (!hasKey(path))
        throw runtime_error("Key not found");

    sqlexec("BEGIN;");
    sqlexec("DELETE FROM kvpair WHERE key = \"%s\";", path.c_str());
    sqlexec("COMMIT;");
}

set<string>
ServerDB::list(const string &path)
{
    int depth = dirDepth(path) + 1;
    std::set<string> r;
    SQLStmt s(db, "SELECT key FROM kvpair WHERE key LIKE '%s/%%' AND depth = %d",
            path.c_str(), depth);

    s.step();
    while (s.row()) {
        r.insert(s.str(0));
        s.step();
    }

    return r;
}

void
ServerDB::create(const char *file)
{
    int status;

    status = sqlite3_open_v2(file,
                             &db,
                             SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
                             NULL);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }

    sqlexec("BEGIN;");
    sqlexec("CREATE TABLE kvpair (key TEXT PRIMARY KEY NOT NULL, value TEXT, depth INT);");
    sqlexec("COMMIT;");
}

void
ServerDB::open(const char *file)
{
    int status;

    status = access(file, F_OK);
    if (status < 0 && errno == ENOENT) {
        create(file);
        return;
    }

    status = sqlite3_open_v2(file,
                             &db,
                             SQLITE_OPEN_READWRITE,
                             NULL);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }
}

void
ServerDB::close()
{
    int status = sqlite3_close(db);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }
}

void
ServerDB::sqlexec(const char *fmt, ...)
{
    char *query;
    va_list ap;

    va_start (ap, fmt);
    query = sqlite3_vmprintf (fmt, ap);
    unique_ptr<char, decltype(&sqlite3_free)> _c(query, sqlite3_free);
    va_end (ap);

    if (!query)
        throw runtime_error("sqlite3_vmprintf: out of memory in sqlexec");

    int err = sqlite3_exec (db, query, NULL, NULL, NULL);
    if (err != SQLITE_OK && err != SQLITE_DONE && err != SQLITE_ROW) {
        cerr << sqlite3_errmsg(db) << endl;
        throw runtime_error("sqlite3_exec: ");
    }
}

#if 0

int
main(int argc, const char *argv[])
{
    ServerDB s("test.db");

    s.set("/test", "myval");
    s.get("/test");
    s.set("/test", "newval");
    s.get("/test");
    s.set("/test/a", "");
    s.set("/test/b", "");
    set<string> lst = s.list("/test");
    for (auto it = lst.begin(); it != lst.end(); it++) {
        cout << "LIST: " << (*it) << endl;
    }
    if (s.hasKey("/bob"))
        cout << "Good 'bob' not present" << endl;
}

#endif

