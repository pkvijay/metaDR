#ifndef _INMEMOPLOGHANDLER_H
#define _INMEMOPLOGHANDLER_H

#include "opLogHandler.h"

#include <mutex>
#include <unordered_map>
#include <vector>

namespace vr {

class InMemOpLogHandler : public IOpLogHandler {
public:
   void appendUpdate(const kvs::KvPair& kvPair, OpInt opNum);
   void appendDelete(const std::string& key, OpInt opNum);
   size_t logSize();
   LogEntry getLogEntry(OpInt opNum);
   OpLog getLog();
   void setLog(const OpLog& opLog);
   bool hasEntry(OpInt opNum);
   void commit(OpInt opNum);

private:
   std::unordered_map<OpInt, LogEntry> _opLog;
   std::vector<OpInt> _opNums;
   std::recursive_mutex _logMutex;

};

} //namespace vr

#endif
