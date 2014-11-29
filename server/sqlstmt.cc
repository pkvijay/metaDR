
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <memory>
#include <sstream>

#include "sqlite3.h"
#include "sqlstmt.h"

using namespace std;

static void
dbthrow (sqlite3 *db, const char *query)
{
  const char *dbpath = sqlite3_db_filename (db, "main");
  if (!dbpath)
    dbpath = "sqlite3 database";
  ostringstream errbuf;
  if (query)
    errbuf << dbpath << ":\n  Query: " << query
	   << "\n  Error: " << sqlite3_errmsg (db);
  else
    errbuf << dbpath << ": " << sqlite3_errmsg (db);
  throw sqlerr_t (errbuf.str ());
}

SQLStmt &
SQLStmt::set_status (int status)
{
  status_ = status;
  if (status != SQLITE_OK && status != SQLITE_ROW && status != SQLITE_DONE)
    dbthrow (sqlite3_db_handle (stmt_), nullptr);
  return *this;
}

void
SQLStmt::fail ()
{
  assert (status_ != SQLITE_OK);
  if (status_ == SQLITE_DONE)
    throw sqldone_t(string ("No rows left in query: ") + sqlite3_sql (stmt_));
  else
    throw sqlerr_t(string ("SQLStmt::operator[]: used after error\n"
			   "  Query: ") + sqlite3_sql (stmt_)
		   + "\n  Error: " + sqlite3_errstr(status_));
}

SQLStmt::SQLStmt (sqlite3 *db, const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  char *query = sqlite3_vmprintf(fmt, ap);
  va_end (ap);

  if (!query)
    throw sqlerr_t ("sqlite3_vmprintf: out of memory");
  unique_ptr<char, decltype(&sqlite3_free)> _c (query, sqlite3_free);

  const char *tail;
  if (sqlite3_prepare_v2(db, query, -1, &stmt_, &tail))
    dbthrow (db, query);
  if (tail && *tail)
    throw sqlerr_t (string("illegal compound query\n  Query:  ") + query);
}

SQLStmt::SQLStmt(const SQLStmt &l)
{
  int err = sqlite3_prepare_v2(sqlite3_db_handle(l.stmt_),
			       sqlite3_sql(l.stmt_), -1, &stmt_, nullptr);
  if (err)
    throw sqlerr_t (string("could not copy query\n  Query:  ")
		    + sqlite3_sql(l.stmt_) + "\n  Error:  "
		    + sqlite3_errstr(err) + "\n");
}

