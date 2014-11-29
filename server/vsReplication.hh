// -*- C++ -*-
// Automatically generated from server/vsReplication.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_VSREPLICATION_HH_INCLUDED__
#define __XDR_SERVER_VSREPLICATION_HH_INCLUDED__ 1

#include <xdrpp/types.h>

 #include "include/kvStore.hh"
 #include "vrTypes.hh"

namespace vr {

struct PrepInfo {
  ViewInt viewNum{};
  kvs::KvPair kvPair{};
  OpInt opNum{};
  OpType opType{};
  CommitInt commitNum{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::PrepInfo>
  : xdr_struct_base<field_ptr<::vr::PrepInfo,
                              decltype(::vr::PrepInfo::viewNum),
                              &::vr::PrepInfo::viewNum>,
                    field_ptr<::vr::PrepInfo,
                              decltype(::vr::PrepInfo::kvPair),
                              &::vr::PrepInfo::kvPair>,
                    field_ptr<::vr::PrepInfo,
                              decltype(::vr::PrepInfo::opNum),
                              &::vr::PrepInfo::opNum>,
                    field_ptr<::vr::PrepInfo,
                              decltype(::vr::PrepInfo::opType),
                              &::vr::PrepInfo::opType>,
                    field_ptr<::vr::PrepInfo,
                              decltype(::vr::PrepInfo::commitNum),
                              &::vr::PrepInfo::commitNum>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::PrepInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.kvPair, "kvPair");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.opType, "opType");
    archive(ar, obj.commitNum, "commitNum");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::PrepInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.kvPair, "kvPair");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.opType, "opType");
    archive(ar, obj.commitNum, "commitNum");
  }
};
} namespace vr {

struct CommitInfo {
  ViewInt viewNum{};
  CommitInt commitNum{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::CommitInfo>
  : xdr_struct_base<field_ptr<::vr::CommitInfo,
                              decltype(::vr::CommitInfo::viewNum),
                              &::vr::CommitInfo::viewNum>,
                    field_ptr<::vr::CommitInfo,
                              decltype(::vr::CommitInfo::commitNum),
                              &::vr::CommitInfo::commitNum>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::CommitInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.commitNum, "commitNum");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::CommitInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.commitNum, "commitNum");
  }
};
} namespace vr {

struct PrepResponse {
  ViewInt viewNum{};
  OpInt opNum{};
  ReplicaId replicaId{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::PrepResponse>
  : xdr_struct_base<field_ptr<::vr::PrepResponse,
                              decltype(::vr::PrepResponse::viewNum),
                              &::vr::PrepResponse::viewNum>,
                    field_ptr<::vr::PrepResponse,
                              decltype(::vr::PrepResponse::opNum),
                              &::vr::PrepResponse::opNum>,
                    field_ptr<::vr::PrepResponse,
                              decltype(::vr::PrepResponse::replicaId),
                              &::vr::PrepResponse::replicaId>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::PrepResponse &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.replicaId, "replicaId");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::PrepResponse &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.replicaId, "replicaId");
  }
};
} namespace vr {

struct ReplicationApi_v1 {
  static constexpr std::uint32_t program = 1074036872;
  static constexpr const char *program_name = "ReplicationApi";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "ReplicationApi_v1";

  struct prepare_t {
    using interface_type = ReplicationApi_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "prepare";
    using arg_type = PrepInfo;
    using arg_wire_type = PrepInfo;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.prepare(std::forward<A>(a)...)) {
      return c.prepare(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.prepare(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.prepare(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct prepareOk_t {
    using interface_type = ReplicationApi_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "prepareOk";
    using arg_type = PrepResponse;
    using arg_wire_type = PrepResponse;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.prepareOk(std::forward<A>(a)...)) {
      return c.prepareOk(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.prepareOk(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.prepareOk(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct commit_t {
    using interface_type = ReplicationApi_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "commit";
    using arg_type = CommitInfo;
    using arg_wire_type = CommitInfo;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.commit(std::forward<A>(a)...)) {
      return c.commit(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.commit(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.commit(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<prepare_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<prepareOk_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<commit_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    prepare(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<prepare_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<prepare_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    prepareOk(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<prepareOk_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<prepareOk_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    commit(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<commit_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<commit_t>(_xdr_args...);
    }
  };
};

}

#endif // !__XDR_SERVER_VSREPLICATION_HH_INCLUDED__
