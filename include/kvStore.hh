// -*- C++ -*-
// Automatically generated from include/kvStore.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_KVSTORE_HH_INCLUDED__
#define __XDR_INCLUDE_KVSTORE_HH_INCLUDED__ 1

#include <xdrpp/types.h>

namespace kvs {

using LongString = xdr::xstring<>;

struct KvPair {
  xdr::xstring<512> key{};
  xdr::xstring<> val{};
};
} namespace xdr {
template<> struct xdr_traits<::kvs::KvPair>
  : xdr_struct_base<field_ptr<::kvs::KvPair,
                              decltype(::kvs::KvPair::key),
                              &::kvs::KvPair::key>,
                    field_ptr<::kvs::KvPair,
                              decltype(::kvs::KvPair::val),
                              &::kvs::KvPair::val>> {
  template<typename Archive> static void
  save(Archive &ar, const ::kvs::KvPair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
  template<typename Archive> static void
  load(Archive &ar, ::kvs::KvPair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
};
} namespace kvs {

enum Error : std::uint32_t {
  NONE,
  KEY_ALREADY_PRESENT,
  KEY_NOT_PRESENT,
  KEY_HAS_NO_PARENT,
  KEY_HAS_CHILDREN,
  KEY_MALFORMED,
  REPLICA_NOT_PRIMARY,
  QUORUM_NOT_REACHED,
  STATUS_NOT_NORMAL,
};
} namespace xdr {
template<> struct xdr_traits<::kvs::Error>
  : xdr_integral_base<::kvs::Error, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::kvs::Error val) {
    switch (val) {
    case ::kvs::NONE:
      return "NONE";
    case ::kvs::KEY_ALREADY_PRESENT:
      return "KEY_ALREADY_PRESENT";
    case ::kvs::KEY_NOT_PRESENT:
      return "KEY_NOT_PRESENT";
    case ::kvs::KEY_HAS_NO_PARENT:
      return "KEY_HAS_NO_PARENT";
    case ::kvs::KEY_HAS_CHILDREN:
      return "KEY_HAS_CHILDREN";
    case ::kvs::KEY_MALFORMED:
      return "KEY_MALFORMED";
    case ::kvs::REPLICA_NOT_PRIMARY:
      return "REPLICA_NOT_PRIMARY";
    case ::kvs::QUORUM_NOT_REACHED:
      return "QUORUM_NOT_REACHED";
    case ::kvs::STATUS_NOT_NORMAL:
      return "STATUS_NOT_NORMAL";
    default:
      return nullptr;
    }
  }
};
} namespace kvs {

struct Result {
private:
  std::uint32_t discriminant_;
  union {
    xdr::xstring<> val_;
    xdr::xvector<LongString> keys_;
    Error err_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 1 ? 1
      : which == 2 ? 2
      : 3;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 1:
      _f(&Result::val_, std::forward<A>(a)...);
      return true;
    case 2:
      _f(&Result::keys_, std::forward<A>(a)...);
      return true;
    default:
      _f(&Result::err_, std::forward<A>(a)...);
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in Result");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~Result();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  Result(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  Result(const Result &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  Result(Result &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~Result() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  Result &operator=(const Result &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~Result();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  Result &operator=(Result &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~Result();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  Result &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  xdr::xstring<> &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("Result: val accessed when not selected");
  }
  const xdr::xstring<> &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("Result: val accessed when not selected");
  }
  xdr::xvector<LongString> &keys() {
    if (_xdr_field_number(discriminant_) == 2)
      return keys_;
    throw xdr::xdr_wrong_union("Result: keys accessed when not selected");
  }
  const xdr::xvector<LongString> &keys() const {
    if (_xdr_field_number(discriminant_) == 2)
      return keys_;
    throw xdr::xdr_wrong_union("Result: keys accessed when not selected");
  }
  Error &err() {
    if (_xdr_field_number(discriminant_) == 3)
      return err_;
    throw xdr::xdr_wrong_union("Result: err accessed when not selected");
  }
  const Error &err() const {
    if (_xdr_field_number(discriminant_) == 3)
      return err_;
    throw xdr::xdr_wrong_union("Result: err accessed when not selected");
  }
};
} namespace xdr {
template<> struct xdr_traits<::kvs::Result> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::kvs::Result;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 1 ? "val"
      : which == 2 ? "keys"
      : "err";
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::kvs::Result &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in Result");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::kvs::Result &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in Result");
  }
  template<typename Archive> static void
  load(Archive &ar, ::kvs::Result &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
} namespace kvs {

struct KvStoreApi_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "KvStoreApi";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "KvStoreApi_v1";

  struct create_t {
    using interface_type = KvStoreApi_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "create";
    using arg_type = KvPair;
    using arg_wire_type = KvPair;
    using res_type = Result;
    using res_wire_type = Result;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.create(std::forward<A>(a)...)) {
      return c.create(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct remove_t {
    using interface_type = KvStoreApi_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "remove";
    using arg_type = LongString;
    using arg_wire_type = LongString;
    using res_type = Result;
    using res_wire_type = Result;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.remove(std::forward<A>(a)...)) {
      return c.remove(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct set_t {
    using interface_type = KvStoreApi_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "set";
    using arg_type = KvPair;
    using arg_wire_type = KvPair;
    using res_type = Result;
    using res_wire_type = Result;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.set(std::forward<A>(a)...)) {
      return c.set(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct get_t {
    using interface_type = KvStoreApi_v1;
    static constexpr std::uint32_t proc = 4;
    static constexpr const char *proc_name = "get";
    using arg_type = LongString;
    using arg_wire_type = LongString;
    using res_type = Result;
    using res_wire_type = Result;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.get(std::forward<A>(a)...)) {
      return c.get(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct list_t {
    using interface_type = KvStoreApi_v1;
    static constexpr std::uint32_t proc = 5;
    static constexpr const char *proc_name = "list";
    using arg_type = LongString;
    using arg_wire_type = LongString;
    using res_type = Result;
    using res_wire_type = Result;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.list(std::forward<A>(a)...)) {
      return c.list(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<create_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<remove_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<set_t>(std::forward<A>(a)...);
      return true;
    case 4:
      t.template dispatch<get_t>(std::forward<A>(a)...);
      return true;
    case 5:
      t.template dispatch<list_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    create(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<create_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<create_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    remove(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<remove_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<remove_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    set(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<set_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<set_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    get(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<get_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<get_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    list(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<list_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<list_t>(_xdr_args...);
    }
  };
};

}

#endif // !__XDR_INCLUDE_KVSTORE_HH_INCLUDED__
