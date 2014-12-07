#include "configurator.h"
#include "kvMemStore.h"
#include "inMemOpLogHandler.h"

#include <mutex>
#include <iostream>
#include <fstream>
#include <climits>
#include <cassert>

using namespace std;
using namespace kvs;

namespace vr {

Configurator* Configurator::_instance = nullptr;

static recursive_mutex configMutex;

Configurator*
Configurator::getInstance()
{
   lock_guard<recursive_mutex> lg(configMutex);
   if (_instance == nullptr) {
      _instance = new Configurator();
   }
   return _instance;
}

void
Configurator::deleteInstance()
{
   lock_guard<recursive_mutex> lg(configMutex);
   if (_instance != nullptr) {
      delete _instance;
   }
}

Configurator::Configurator() : _replicaId(UINT_MAX),
                               _replicator(nullptr) {}

/*
 * TODO: Use regex for below input file parsing.
 *       Do case insensitive comparison
 */

// trim the line of beginning and trailing whitespaces
static string&
trim(string& line)
{
   size_t pos = line.find_first_not_of(' ');
   if (pos != string::npos) {
      line.erase(0, pos);
   }
   pos = line.find_last_not_of(' ');
   if (pos != string::npos) {
      line.erase(pos + 1, line.size() - pos - 1);
   }
   return line;
}

static void
split(const string& line, char delim,
      std::vector<std::string>& res)
{
   size_t pos = 0;
   for (size_t i = 0; i < line.size(); ++i) {
      if (line.at(i) == delim && pos < i) {
         res.push_back(line.substr(pos, i - pos));
         trim(res.at(res.size() - 1));
         pos = i + 1;
      }
   }
   res.push_back(line.substr(pos, line.size() - pos));
   trim(res.at(res.size() - 1));
}

struct UserInput {
   std::string clientPort;
   std::string replicationPort;

   /*
    * Replica endpoints.
    * One of the entries in the below array should be localhost:replicationPort.
    * The position of a replica endpoint in the below array indicates the
    * replica ID.
    */
   std::vector<EndPoint> endPoints;
   bool recovering;
};

static void
parseLine(string& line, UserInput& uip)
{
   trim(line);

   if (line.empty()) {
      return;
   }

   // ignore comment.
   if (line.find_first_of('#') == 0) {
      return;
   }

   vector<string> elems;
   split(line, '=', elems);
   if (elems.size() != 2) {
      cerr << "Line, '" << line
           << "' in config file is not in key = val format" << endl;
      throw invalid_argument(line);
   }
   const string& key = elems.at(0);
   const string& val = elems.at(1);

   if (key == "ClientPort") {
      uip.clientPort = val;
   }
   else if (key == "ReplicationPort") {
      uip.replicationPort = val;
   }
   else if (key == "EndPoints") {
      vector<string> endPts;
      split(val, ',', endPts);
      for (size_t i = 0; i < endPts.size(); ++i) {
         vector<string> res;
         split(endPts.at(i), ':', res);
         if (res.size() != 2) {
            cerr << "Line, '" << line
                 << "' does not have a endpoint in a valid ip:port format"
                 << endl;
            throw invalid_argument(line);
         }
         EndPoint endPoint(res.at(0), res.at(1));
         uip.endPoints.push_back(endPoint);
      }
   }
   else if (key == "Recovering") {
      if (val == "true") {
         uip.recovering = true;
      }
   }
   else {
      cerr << "Line, '" << line << "' in config file is not valid" << endl;
      throw invalid_argument(line);
   }
}

static void
validate(const UserInput& uip)
{
   /*
    * TODO: Add the required input validation.
    *       1. IP address and port num validation
    *       2. Validate whether all uip entries are populated
    *       3. Validate if endPoints have an entry of localhost::replicationPort
    */
}

struct FileReader {
   FileReader(const string& filePath) : ifs(filePath, ifstream::in) {}
   ~FileReader() {
      ifs.close();
   }

   ifstream ifs;
};

static void
parse(const string& filePath, UserInput& uip)
{
   {
      FileReader fr(filePath);
      while(fr.ifs.good()) {
         string line;
         getline(fr.ifs, line);
         parseLine(line, uip);
      }
   }
   validate(uip);
}

void
Configurator::bootStrap(const string& cfgFile)
{
   lock_guard<recursive_mutex> lg(configMutex);
   UserInput uip;
   parse(cfgFile, uip);

   // TODO: try using a move below instead of copy
   _clientPort = uip.clientPort;
   _replicationPort = uip.replicationPort;

   for (size_t i = 0; i < uip.endPoints.size(); ++i) {
      const EndPoint& ep = uip.endPoints.at(i);
      if ((ep.ip == "localhost" || ep.ip == "127.0.0.1") &&
          ep.port == _replicationPort) {
         _replicaId = i;
      }
      else {
         _replicas[i] = ep;
      }
   }

   assert(!_clientPort.empty());
   assert(!_replicationPort.empty());
   assert(_replicaId != UINT_MAX);

   // bootstrap should be done only once.
   assert(_replicator == nullptr);

   KvMemStore* kvStore = KvMemStore::getInstance();
   _replicator = new VsReplicator(_replicaId, _replicas,
                                  new InMemOpLogHandler());
   kvStore->registerReplicator(_replicator);
  _replicator->start(uip.recovering);
}

ReplicaId
Configurator::getReplicaId() const
{
   lock_guard<recursive_mutex> lg(configMutex);
   return _replicaId;
}

const string&
Configurator::getClientPort() const
{
   lock_guard<recursive_mutex> lg(configMutex);
   return _clientPort;
}

const string&
Configurator::getReplicationPort() const
{
   lock_guard<recursive_mutex> lg(configMutex);
   return _replicationPort;
}

const unordered_map<ReplicaId, EndPoint>&
Configurator::getReplicas() const
{
   lock_guard<recursive_mutex> lg(configMutex);
   return _replicas;
}

const EndPoint&
Configurator::getReplicaEndPoint(ReplicaId id) const
{
   lock_guard<recursive_mutex> lg(configMutex);
   auto it = _replicas.find(id);
   if (it == _replicas.end()) {
      cerr << "Cannot find replica with id, " << id << endl;
      throw invalid_argument("Invalid replica Id");
   }
   return it->second;
}

VsReplicator*
Configurator::getReplicator()
{
   lock_guard<recursive_mutex> lg(configMutex);
   assert(_replicator != nullptr);
   return _replicator;
}

} //namespace vr
