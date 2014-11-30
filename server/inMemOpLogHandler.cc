#include "inMemOpLogHandler.h"

#include <iostream>
#include <cassert>

using namespace std;
using namespace kvs;

namespace vr {

void
InMemOpLogHandler::appendUpdate(const KvPair& kvPair, OpInt opNum)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   LogEntry entry;
   entry.kvPair = kvPair;
   entry.opType = UPDATE;
   entry.opNum = opNum;
   cout << "Adding update entry, opNum : " << opNum << endl;
   _opNums.push_back(opNum);
   _opLog[opNum] = entry;
}

void
InMemOpLogHandler::appendDelete(const string& key, OpInt opNum)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   KvPair kvPair;
   kvPair.key = key;
   LogEntry entry;
   entry.kvPair = move(kvPair);
   entry.opType = DELETE;
   entry.opNum = opNum;
   cout << "Adding delete entry, opNum : " << opNum << endl;
   _opNums.push_back(opNum);
   _opLog[opNum] = entry;
}

size_t
InMemOpLogHandler::logSize()
{
   lock_guard<recursive_mutex> lg(_logMutex);
   return _opLog.size();
}

LogEntry
InMemOpLogHandler::getLogEntry(OpInt opNum)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   auto it = _opLog.find(opNum);
   assert (it != _opLog.end());
   return it->second;
}

OpLog
InMemOpLogHandler::getLog()
{
   lock_guard<recursive_mutex> lg(_logMutex);
   OpLog opLog;
   for (size_t i = 0; i < _opNums.size(); ++i) {
      OpInt opNum = _opNums.at(i);
      opLog.push_back(_opLog[opNum]);
   }
   return opLog;
}

void
InMemOpLogHandler::setLog(const OpLog& opLog)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   _opNums.clear();
   _opLog.clear();
   for (size_t i = 0; i < opLog.size(); ++i) {
      OpInt opNum = opLog.at(i).opNum;
      _opNums.push_back(opNum);
      _opLog[opNum] =  opLog.at(i);
   }
}

bool
InMemOpLogHandler::hasEntry(OpInt opNum)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   return (_opLog.find(opNum) != _opLog.end());
}

void
InMemOpLogHandler::commit(OpInt opNum)
{
   lock_guard<recursive_mutex> lg(_logMutex);
   auto it = _opLog.find(opNum);
   assert(it != _opLog.end());
   LogEntry& entry = it->second;
   entry.committed = true;
}


} // namespace vr
