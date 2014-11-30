#include "vsReplicaState.h"

using namespace std;

namespace vr {

ReplicaId
ReplicaState::priId()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _priId;
}

Status
ReplicaState::status()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _status;
}

ViewInt
ReplicaState::viewNum()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _viewNum;
}

OpInt
ReplicaState::opNum()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _opNum;
}

CommitInt
ReplicaState::commitNum()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _commitNum;
}

void
ReplicaState::priId(ReplicaId priId_)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _priId = priId_;
}

void
ReplicaState::status(Status status_)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _status = status_;
}

void
ReplicaState::viewNum(ViewInt viewNum_)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _viewNum = viewNum_;
}

void
ReplicaState::opNum(OpInt opNum_)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _opNum = opNum_;
}

void
ReplicaState::commitNum(CommitInt commitNum_)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _commitNum = commitNum_;
}

ViewInt
ReplicaState::incrementViewNum()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return ++_viewNum;
}

OpInt
ReplicaState::incrementOpNum()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return ++_opNum;
}

chrono::time_point<chrono::system_clock>
ReplicaState::lastCommitTime()
{
   lock_guard<recursive_mutex> lg(rsMutex);
   return _lastCommitTime;
}

void
ReplicaState::lastCommitTime(
   chrono::time_point<chrono::system_clock> lastCommitTime)
{
   lock_guard<recursive_mutex> lg(rsMutex);
   _lastCommitTime = lastCommitTime;
}

} //namespace vr
