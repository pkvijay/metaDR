// -*- C++ -*-
// Automatically generated from server/recovery.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_RECOVERY_HH_INCLUDED__
#define __XDR_SERVER_RECOVERY_HH_INCLUDED__ 1

#include <xdrpp/types.h>

 #include "vrTypes.hh"

namespace vr {

using Nonce = std::uint64_t;

struct RecoveryRequest {
  ReplicaId replicaId{};
  Nonce nonce{};
  CommitInt knownCommitNum{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::RecoveryRequest>
  : xdr_struct_base<field_ptr<::vr::RecoveryRequest,
                              decltype(::vr::RecoveryRequest::replicaId),
                              &::vr::RecoveryRequest::replicaId>,
                    field_ptr<::vr::RecoveryRequest,
                              decltype(::vr::RecoveryRequest::nonce),
                              &::vr::RecoveryRequest::nonce>,
                    field_ptr<::vr::RecoveryRequest,
                              decltype(::vr::RecoveryRequest::knownCommitNum),
                              &::vr::RecoveryRequest::knownCommitNum>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::RecoveryRequest &obj) {
    archive(ar, obj.replicaId, "replicaId");
    archive(ar, obj.nonce, "nonce");
    archive(ar, obj.knownCommitNum, "knownCommitNum");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::RecoveryRequest &obj) {
    archive(ar, obj.replicaId, "replicaId");
    archive(ar, obj.nonce, "nonce");
    archive(ar, obj.knownCommitNum, "knownCommitNum");
  }
};
} namespace vr {

struct RecoveryResponse {
  ViewInt viewNum{};
  Nonce nonce{};
  ReplicaId replicaId{};
  bool isPrimary{};
  OpLog opLog{};
  OpInt opNum{};
  CommitInt commitNum{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::RecoveryResponse>
  : xdr_struct_base<field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::viewNum),
                              &::vr::RecoveryResponse::viewNum>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::nonce),
                              &::vr::RecoveryResponse::nonce>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::replicaId),
                              &::vr::RecoveryResponse::replicaId>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::isPrimary),
                              &::vr::RecoveryResponse::isPrimary>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::opLog),
                              &::vr::RecoveryResponse::opLog>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::opNum),
                              &::vr::RecoveryResponse::opNum>,
                    field_ptr<::vr::RecoveryResponse,
                              decltype(::vr::RecoveryResponse::commitNum),
                              &::vr::RecoveryResponse::commitNum>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::RecoveryResponse &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.nonce, "nonce");
    archive(ar, obj.replicaId, "replicaId");
    archive(ar, obj.isPrimary, "isPrimary");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::RecoveryResponse &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.nonce, "nonce");
    archive(ar, obj.replicaId, "replicaId");
    archive(ar, obj.isPrimary, "isPrimary");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
  }
};
} namespace vr {

struct RecoveryApi_v1 {
  static constexpr std::uint32_t program = 1074036994;
  static constexpr const char *program_name = "RecoveryApi";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "RecoveryApi_v1";

  struct recoveryRequest_t {
    using interface_type = RecoveryApi_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "recoveryRequest";
    using arg_type = RecoveryRequest;
    using arg_wire_type = RecoveryRequest;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.recoveryRequest(std::forward<A>(a)...)) {
      return c.recoveryRequest(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.recoveryRequest(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.recoveryRequest(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct recoveryResponse_t {
    using interface_type = RecoveryApi_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "recoveryResponse";
    using arg_type = RecoveryResponse;
    using arg_wire_type = RecoveryResponse;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.recoveryResponse(std::forward<A>(a)...)) {
      return c.recoveryResponse(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.recoveryResponse(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.recoveryResponse(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<recoveryRequest_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<recoveryResponse_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    recoveryRequest(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<recoveryRequest_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<recoveryRequest_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    recoveryResponse(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<recoveryResponse_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<recoveryResponse_t>(_xdr_args...);
    }
  };
};

}

#endif // !__XDR_SERVER_RECOVERY_HH_INCLUDED__
