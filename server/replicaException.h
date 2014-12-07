#ifndef _REPLICAEXCEPTION_H
#define _REPLICAEXCEPTION_H

#include <string>

class QuorumNotReached : public std::runtime_error {
public:
   QuorumNotReached(const std::string& msg) : std::runtime_error(msg) {}
};


class ReplicaStateNotNormal : public std::runtime_error {
public:
   ReplicaStateNotNormal(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
