#ifndef _VSREPLICASTATE_H
#define _VSREPLICASTATE_H

#include <mutex>
#include <unordered_set>

#include "vrTypes.hh"

namespace vr {

class ReplicaState {
public:
   ReplicaState() : _priId(0), _status(NORMAL), _viewNum(0),
                    _opNum(0), _commitNum(0),
                    _lastCommitTime(std::chrono::system_clock::now()) {}

   ReplicaState(ReplicaId priId, Status status, ViewInt viewNum,
                OpInt opNum, CommitInt commitNum) :
                  _priId(priId), _status(status), _viewNum(viewNum),
                  _opNum(opNum), _commitNum(commitNum),
                  _lastCommitTime(std::chrono::system_clock::now()) {}

   ReplicaId priId();
   Status status();
   ViewInt viewNum();
   OpInt opNum();
   CommitInt commitNum();
   std::chrono::time_point<std::chrono::system_clock> lastCommitTime();

   void priId(ReplicaId priId_);
   void status(Status status_);
   void viewNum(ViewInt viewNum_);
   void opNum(OpInt opNum_);
   void commitNum(CommitInt commitNum_);
   void lastCommitTime(
      std::chrono::time_point<std::chrono::system_clock> lastCommitTime);

   ViewInt incrementViewNum();
   OpInt incrementOpNum();

private:
   ReplicaId _priId;
   Status _status;
   ViewInt _viewNum;
   OpInt _opNum;
   CommitInt _commitNum;
   std::chrono::time_point<std::chrono::system_clock> _lastCommitTime;
   std::recursive_mutex rsMutex;
};

} //namespace vr

#endif
