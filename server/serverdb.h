
#ifndef __SERVERDB_H__
#define __SERVERDB_H__

#include "sqlite3.h"

#include <set>

class ServerDB {
public:
    /*
     * Constructor for ServerDB
     *
     * Creates an instance backed by the given file name
     */
    ServerDB(const char *file);
    /*
     * Destructor for ServerDB
     */
    ~ServerDB();
    /*
     * Checks if a key exists.
     *
     * Return Value: true if the key exists, false otherwise
     */
    bool hasKey(const std::string &path);
    /*
     * Get a value
     *
     * Return Value: value of the key or throws an exception
     */
    std::string get(const std::string &path);
    /*
     * Set or Update a key-value pair
     *
     * Return Value: true if the key already exists, false otherwise
     */
    bool set(const std::string &path, const std::string &val);
    /*
     * Removes a single key
     *
     * Throws an exception if it does not exist
     */
    void remove(const std::string &path);
    /*
     * List all sub-keys
     *
     * Return Value: A set of all sub-keys, returns empty set if it does not
     * exist or has no children.
     */
    std::set<std::string> list(const std::string &path);
private:
    // Helper functions
    void create(const char *file);
    void open(const char *file);
    void close();
    void sqlexec(const char *fmt, ...);
    sqlite3 *db;
};

#endif /* __SERVERDB_H__ */

