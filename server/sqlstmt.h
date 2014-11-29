
#ifndef __SQLSTMT_H__
#define __SQLSTMT_H__

#include <assert.h>

#include <string>
#include <tuple>

#include "sqlite3.h"

using i64 = sqlite3_int64;

struct sqlerr_t : public std::runtime_error {
  sqlerr_t (const std::string &msg) : std::runtime_error (msg) {}
};
/* A sqldone_t is thrown if you ask for data when no rows are left */
struct sqldone_t : public std::runtime_error {
  sqldone_t (const std::string &msg) : std::runtime_error (msg) {}
};

class SQLStmt {
  sqlite3_stmt *stmt_;
  int status_ = SQLITE_OK;
  SQLStmt &set_status (int status);
  void fail ();
  void ensure_row () { if (status_ != SQLITE_ROW) fail(); }

 public:
  explicit SQLStmt(sqlite3_stmt *stmt) : stmt_(stmt) {}
  explicit SQLStmt(sqlite3 *db, const char *fmt, ...);
  SQLStmt(const SQLStmt &r);
  SQLStmt(SQLStmt &&r) : stmt_ (r.stmt_) { r.stmt_ = nullptr; }
  ~SQLStmt() { sqlite3_finalize (stmt_); }

  sqlite3_stmt *get() { return stmt_; }
  sqlite3 *getdb() { return sqlite3_db_handle(stmt_); }
  int status() const { return status_; }
  bool row() {
    if (status_ == SQLITE_ROW)
      return true;
    // Something like SQLITE_OK indicates row() not used after step()
    assert (status_ == SQLITE_DONE);
    return false;
  }
  bool done() { return !row(); }
  SQLStmt &step() { return set_status(sqlite3_step (stmt_)); }
  SQLStmt &reset() { return set_status(sqlite3_reset (stmt_)); }

  /* Access columns */
  template<typename T> T column(int);
  bool null(int i) {
    ensure_row();
    return sqlite3_column_type (stmt_, i) == SQLITE_NULL;
  }
  sqlite3_int64 integer(int i) {
    ensure_row();
    return sqlite3_column_int64 (stmt_, i);
  }
  double real(int i) {
    ensure_row();
    return sqlite3_column_double (stmt_, i);
  }
  std::string str(int i) {
    ensure_row();
    return { static_cast<const char *> (sqlite3_column_blob (stmt_, i)),
	size_t (sqlite3_column_bytes (stmt_, i)) };
  }
  const char *c_str(int i) {
    ensure_row();
    return reinterpret_cast<const char *> (sqlite3_column_text (stmt_, i));
  }
  sqlite3_value *value(int i) {
    ensure_row();
    return sqlite3_column_value(stmt_, i);
  }

  /* Bind parameters */
  SQLStmt &bind_null(int i) {
    return set_status (sqlite3_bind_null(stmt_, i));
  }
  SQLStmt &bind_int(int i, sqlite3_int64 v) {
    return set_status (sqlite3_bind_int64(stmt_, i, v));
  }
  SQLStmt &bind_real(int i, double v) {
    return set_status (sqlite3_bind_double(stmt_, i, v));
  }
  SQLStmt &bind_text(int i, const std::string &v) {
    return set_status (sqlite3_bind_text(stmt_, i, v.data(), v.size(),
					 SQLITE_STATIC));
  }
  SQLStmt &bind_text(int i, std::string &&v) {
    return set_status (sqlite3_bind_text(stmt_, i, v.data(), v.size(),
					 SQLITE_TRANSIENT));
  }
  SQLStmt &bind_text(int i, const char *p, int len = -1) {
    return set_status (sqlite3_bind_text(stmt_, i, p, len, SQLITE_STATIC));
  }
  SQLStmt &bind_blob(int i, const void *p, int len) {
    return set_status (sqlite3_bind_blob(stmt_, i, p, len, SQLITE_STATIC));
  }
  SQLStmt &bind_value(int i, const sqlite3_value *v) {
    return set_status (sqlite3_bind_value (stmt_, i, v));
  }

  /* Overloaded bind */
  SQLStmt &bind(int i, std::nullptr_t) { return bind_null(i); }
  SQLStmt &bind(int i, sqlite3_int64 v) { return bind_int(i, v); }
  SQLStmt &bind(int i, int v) { return bind_int(i, v); }
  SQLStmt &bind(int i, unsigned v) { return bind_int(i, v); }
  SQLStmt &bind(int i, const double &v) { return bind_real(i, v); }
  SQLStmt &bind(int i, const std::string &v) { return bind_text(i, v); }
  SQLStmt &bind(int i, std::string &&v) { return bind_text(i, std::move(v)); }
  SQLStmt &bind(int i, const char *v) { return bind_text(i, v); }
  SQLStmt &bind(int i, const sqlite3_value *v) { return bind_value(i, v); }

  /* Bind multiple parameters at once */
  SQLStmt &_param(int) { return *this; }
  template<typename H, typename... T>
  SQLStmt &_param(int i, H&& h, T&&... t) {
    return this->bind(i, std::forward<H>(h))._param(i+1, std::forward<T>(t)...);
  }
  template<typename... Args> SQLStmt &param(Args&&... args) {
    return _param (1, std::forward<Args> (args)...);
  }

  /* Bind tuple */
  template<size_t N> struct _tparm_helper {
    template<typename... Args>
    static SQLStmt &go(SQLStmt &s, const std::tuple<Args...> &t) {
      return _tparm_helper<N-1>::go(s.bind(N, std::get<N-1>(t)), t);
    }
  };
  template<typename... Args>
  SQLStmt &tparam(const std::tuple<Args...> &t) {
    return _tparm_helper<sizeof...(Args)>::go(*this, t);
  }
};

template<> struct SQLStmt::_tparm_helper<0> {
  template<typename... Args>
  static SQLStmt &go(SQLStmt &s, const std::tuple<Args...> &t) { return s; }
};

template<> inline bool
SQLStmt::column(int i)
{
  return null(i);
}
template<> inline i64
SQLStmt::column(int i)
{
  return integer(i);
}
template<> inline double
SQLStmt::column(int i)
{
  return real(i);
}
template<> inline std::string
SQLStmt::column(int i)
{
  return str(i);
}
template<> inline const char *
SQLStmt::column(int i)
{
  return c_str(i);
}

#endif

