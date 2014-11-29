#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <set>
#include <string>
#include <fstream>
#include <iostream>

#include "xdrpp/srpc.h"
#include "include/kvStore.hh"
#include "include/kvsClient.h"
#include "include/endPoint.h"

using namespace std;

kvs::Client client;

#define DEBUG_MAX_ARGS      5
#define DEBUG_MAX_LINE      256

void
Cmd_Help(int argc, const char *argv[])
{
    cout << "create     Create a node" << endl;
    cout << "echo       Echo arguments" << endl;
    cout << "exit       Exit shell" << endl;
    cout << "get        Get a node" << endl;
    cout << "help       Display the list of commands" << endl;
    cout << "remove     Remove a node" << endl;
    cout << "set        Set a node" << endl;
}

void
Cmd_Echo(int argc, const char *argv[])
{
    int i;

    for (i = 1; i < argc; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;
}

void
Cmd_Create(int argc, const char *argv[])
{
    if (argc != 3) {
        cout << "Usage: create PATH VALUE" << endl;
        return;
    }

    try {
        if (client.create(argv[1], argv[2]))
            cout << "CREATED" << endl;
        else
            cout << "KEY ALREADY EXISTS" << endl;
    } catch (kvs::ClientException &e) {
        cout << e.what() << endl;
    }
}

void
Cmd_Remove(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Usage: remove PATH" << endl;
        return;
    }

    try {
        if (client.remove(argv[1]))
            cout << "REMOVED" << endl;
        else
            cout << "KEY NOT FOUND" << endl;
    } catch (kvs::ClientException &e) {
        cout << e.what() << endl;
    }
}

void
Cmd_Set(int argc, const char *argv[])
{
    if (argc != 3) {
        cout << "Usage: set PATH VALUE" << endl;
        return;
    }

    try {
        client.set(argv[1], argv[2]);
    } catch (kvs::ClientException &e) {
        cout << e.what() << endl;
    }
}

void
Cmd_Get(int argc, const char *argv[])
{
    string result;

    if (argc != 2) {
        cout << "Usage: get PATH" << endl;
        return;
    }

    try {
        result = client.get(argv[1]);
        cout << result << endl;
    } catch (kvs::ClientException &e) {
        cout << e.what() << endl;
    }
}

void
Cmd_List(int argc, const char *argv[])
{
    set<string> result;

    if (argc != 2) {
        cout << "Usage: list PATH" << endl;
        return;
    }

    try {
        result = client.list(argv[1]);
    } catch (kvs::ClientException &e) {
        cout << e.what() << endl;
        return;
    }

    for (auto it = result.begin(); it != result.end(); it++) {
        cout << (*it) << endl;
    }
}

void
DispatchCommand(char *buf)
{
    int argc;
    char *argv[DEBUG_MAX_ARGS];
    char *nextArg;

    // parse input
    argv[0] = buf;
    for (argc = 1; argc < DEBUG_MAX_ARGS; argc++) {
        nextArg = strchr(argv[argc - 1], ' ');
        if (nextArg == NULL)
        {
            break;
        }

        *nextArg = '\0';
        argv[argc] = nextArg + 1;
    }

    // execute command
    string cmd = argv[0];
    if (cmd == "help") {
        Cmd_Help(argc, (const char **)argv);
    } else if (cmd == "echo") {
        Cmd_Echo(argc, (const char **)argv);
    } else if (cmd == "exit") {
        exit(0);
    } else if (cmd == "create") {
        Cmd_Create(argc, (const char **)argv);
    } else if (cmd == "remove") {
        Cmd_Remove(argc, (const char **)argv);
    } else if (cmd == "get") {
        Cmd_Get(argc, (const char **)argv);
    } else if (cmd == "set") {
        Cmd_Set(argc, (const char **)argv);
    } else if (cmd == "list") {
        Cmd_List(argc, (const char **)argv);
    } else if (cmd == "#") {
        // Ignore comments
    } else if (cmd != "") {
        printf("Unknown command '%s'\n", argv[0]);
    }
}

void
Prompt()
{
    char buf[DEBUG_MAX_LINE];
    cout << "Client Shell" << endl;

    while (cin.good()) {
        cout << "> ";

        // read input
        cin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

void
RunScript(const char *file)
{
    char buf[DEBUG_MAX_LINE];
    fstream fin {file};

    while (!fin.eof()) {
        // read input
        fin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

int
main(int argc, const char *argv[])
{
    client = kvs::Client();

    if (argc != 2 && argc != 3) {
        cout << "Usage: shell HOST [SCRIPT]" << endl;
        return 1;
    }

    // Setup connection
    try {
        EndPoint ep(argv[1]);
        client.open(ep);
    } catch (exception &e) {
        cout << "Connection failed!" << endl;
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    // Either execute script or prompt
    try {
        if (argc == 2) {
            Prompt();
        } else {
            RunScript(argv[2]);
        }
    } catch(exception &e) {
        cout << e.what() << endl;
        client.close();
        return 1;
    }
    client.close();
    return 0;
}

