// -*- C++ -*-
// Automatically generated from server/viewChange.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_VIEWCHANGE_HH_INCLUDED__
#define __XDR_SERVER_VIEWCHANGE_HH_INCLUDED__ 1

#include <xdrpp/types.h>

 #include "vrTypes.hh"

namespace vr {

struct ViewChangeRequest {
  ViewInt viewNum{};
  ReplicaId replicaId{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::ViewChangeRequest>
  : xdr_struct_base<field_ptr<::vr::ViewChangeRequest,
                              decltype(::vr::ViewChangeRequest::viewNum),
                              &::vr::ViewChangeRequest::viewNum>,
                    field_ptr<::vr::ViewChangeRequest,
                              decltype(::vr::ViewChangeRequest::replicaId),
                              &::vr::ViewChangeRequest::replicaId>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::ViewChangeRequest &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.replicaId, "replicaId");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::ViewChangeRequest &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.replicaId, "replicaId");
  }
};
} namespace vr {

struct ViewChangeInfo {
  ViewInt viewNum{};
  OpLog opLog{};
  ViewInt oldViewNum{};
  OpInt opNum{};
  CommitInt commitNum{};
  ReplicaId replicaId{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::ViewChangeInfo>
  : xdr_struct_base<field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::viewNum),
                              &::vr::ViewChangeInfo::viewNum>,
                    field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::opLog),
                              &::vr::ViewChangeInfo::opLog>,
                    field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::oldViewNum),
                              &::vr::ViewChangeInfo::oldViewNum>,
                    field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::opNum),
                              &::vr::ViewChangeInfo::opNum>,
                    field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::commitNum),
                              &::vr::ViewChangeInfo::commitNum>,
                    field_ptr<::vr::ViewChangeInfo,
                              decltype(::vr::ViewChangeInfo::replicaId),
                              &::vr::ViewChangeInfo::replicaId>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::ViewChangeInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.oldViewNum, "oldViewNum");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
    archive(ar, obj.replicaId, "replicaId");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::ViewChangeInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.oldViewNum, "oldViewNum");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
    archive(ar, obj.replicaId, "replicaId");
  }
};
} namespace vr {

struct ViewStartInfo {
  ViewInt viewNum{};
  OpLog opLog{};
  OpInt opNum{};
  CommitInt commitNum{};
  ReplicaId replicaId{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::ViewStartInfo>
  : xdr_struct_base<field_ptr<::vr::ViewStartInfo,
                              decltype(::vr::ViewStartInfo::viewNum),
                              &::vr::ViewStartInfo::viewNum>,
                    field_ptr<::vr::ViewStartInfo,
                              decltype(::vr::ViewStartInfo::opLog),
                              &::vr::ViewStartInfo::opLog>,
                    field_ptr<::vr::ViewStartInfo,
                              decltype(::vr::ViewStartInfo::opNum),
                              &::vr::ViewStartInfo::opNum>,
                    field_ptr<::vr::ViewStartInfo,
                              decltype(::vr::ViewStartInfo::commitNum),
                              &::vr::ViewStartInfo::commitNum>,
                    field_ptr<::vr::ViewStartInfo,
                              decltype(::vr::ViewStartInfo::replicaId),
                              &::vr::ViewStartInfo::replicaId>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::ViewStartInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
    archive(ar, obj.replicaId, "replicaId");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::ViewStartInfo &obj) {
    archive(ar, obj.viewNum, "viewNum");
    archive(ar, obj.opLog, "opLog");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.commitNum, "commitNum");
    archive(ar, obj.replicaId, "replicaId");
  }
};
} namespace vr {

struct ViewChangeApi_v1 {
  static constexpr std::uint32_t program = 1074036992;
  static constexpr const char *program_name = "ViewChangeApi";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "ViewChangeApi_v1";

  struct startViewChange_t {
    using interface_type = ViewChangeApi_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "startViewChange";
    using arg_type = ViewChangeRequest;
    using arg_wire_type = ViewChangeRequest;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.startViewChange(std::forward<A>(a)...)) {
      return c.startViewChange(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.startViewChange(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.startViewChange(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct doViewChange_t {
    using interface_type = ViewChangeApi_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "doViewChange";
    using arg_type = ViewChangeInfo;
    using arg_wire_type = ViewChangeInfo;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.doViewChange(std::forward<A>(a)...)) {
      return c.doViewChange(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.doViewChange(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.doViewChange(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct startView_t {
    using interface_type = ViewChangeApi_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "startView";
    using arg_type = ViewStartInfo;
    using arg_wire_type = ViewStartInfo;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.startView(std::forward<A>(a)...)) {
      return c.startView(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.startView(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.startView(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<startViewChange_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<doViewChange_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<startView_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    startViewChange(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<startViewChange_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<startViewChange_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    doViewChange(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<doViewChange_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<doViewChange_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    startView(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<startView_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<startView_t>(_xdr_args...);
    }
  };
};

}

#endif // !__XDR_SERVER_VIEWCHANGE_HH_INCLUDED__
