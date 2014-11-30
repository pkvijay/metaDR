// -*- C++ -*-
// Automatically generated from server/vrTypes.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_VRTYPES_HH_INCLUDED__
#define __XDR_SERVER_VRTYPES_HH_INCLUDED__ 1

#include <xdrpp/types.h>

 #include "include/kvStore.hh"

namespace vr {

using ViewInt = std::uint64_t;
using OpInt = std::uint64_t;
using ReplicaId = std::uint64_t;
using CommitInt = std::uint64_t;

enum Status : std::uint32_t {
  NORMAL,
  VIEW_CHANGE,
  RECOVERING,
};
} namespace xdr {
template<> struct xdr_traits<::vr::Status>
  : xdr_integral_base<::vr::Status, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::vr::Status val) {
    switch (val) {
    case ::vr::NORMAL:
      return "NORMAL";
    case ::vr::VIEW_CHANGE:
      return "VIEW_CHANGE";
    case ::vr::RECOVERING:
      return "RECOVERING";
    default:
      return nullptr;
    }
  }
};
} namespace vr {

enum OpType : std::uint32_t {
  UPDATE,
  DELETE,
};
} namespace xdr {
template<> struct xdr_traits<::vr::OpType>
  : xdr_integral_base<::vr::OpType, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::vr::OpType val) {
    switch (val) {
    case ::vr::UPDATE:
      return "UPDATE";
    case ::vr::DELETE:
      return "DELETE";
    default:
      return nullptr;
    }
  }
};
} namespace vr {

struct LogEntry {
  kvs::KvPair kvPair{};
  OpType opType{};
  OpInt opNum{};
  bool committed{};
};
} namespace xdr {
template<> struct xdr_traits<::vr::LogEntry>
  : xdr_struct_base<field_ptr<::vr::LogEntry,
                              decltype(::vr::LogEntry::kvPair),
                              &::vr::LogEntry::kvPair>,
                    field_ptr<::vr::LogEntry,
                              decltype(::vr::LogEntry::opType),
                              &::vr::LogEntry::opType>,
                    field_ptr<::vr::LogEntry,
                              decltype(::vr::LogEntry::opNum),
                              &::vr::LogEntry::opNum>,
                    field_ptr<::vr::LogEntry,
                              decltype(::vr::LogEntry::committed),
                              &::vr::LogEntry::committed>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vr::LogEntry &obj) {
    archive(ar, obj.kvPair, "kvPair");
    archive(ar, obj.opType, "opType");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.committed, "committed");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vr::LogEntry &obj) {
    archive(ar, obj.kvPair, "kvPair");
    archive(ar, obj.opType, "opType");
    archive(ar, obj.opNum, "opNum");
    archive(ar, obj.committed, "committed");
  }
};
} namespace vr {

using OpLog = xdr::xvector<LogEntry>;

}

#endif // !__XDR_SERVER_VRTYPES_HH_INCLUDED__
