#ifndef _OPLOGHANDLER_H
#define _OPLOGHANDLER_H

#include <string>

#include "vrTypes.hh"

namespace vr {

class IOpLogHandler {
public:
   virtual ~IOpLogHandler() {};
   virtual void appendUpdate(const kvs::KvPair& kvPair, OpInt opNum) = 0;
   virtual void appendDelete(const std::string& key, OpInt opNum) = 0;
   virtual size_t logSize() = 0;
   virtual LogEntry getLogEntry(OpInt opNum) = 0;
   virtual OpLog getLog() = 0;
   virtual void setLog(const OpLog& opLog) = 0;
   virtual bool hasEntry(OpInt opNum) = 0;
   virtual void commit(OpInt opNum) = 0;

   //TODO: Add log trimming interfaces
};

} //namespace vr

#endif
