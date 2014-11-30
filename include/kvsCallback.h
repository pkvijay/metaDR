#ifndef _KVSCALLBACK_H
#define _KVSCALLBACK_H


namespace kvs {
   using UpdateCbFunc = void(*)(const std::string& key, const std::string& val);
   using DeleteCbFunc = void(*)(const std::string& key);

} // namespace kvs

#endif
