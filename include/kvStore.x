namespace kvs {  // kvs => key-value store
   typedef string LongString<>;

   struct KvPair {
      string key<512>;
      string val<>;
   };

   enum Error {
      NONE,
      KEY_ALREADY_PRESENT,
      KEY_NOT_PRESENT,
      KEY_HAS_NO_PARENT,
      KEY_HAS_CHILDREN,
      KEY_MALFORMED
   };

   union Result switch(unsigned discriminant) {
      case 1:
         string val<>;
      case 2:
         LongString keys<>;
      default:
         Error err;
   };

   program KvStoreApi {
      version KvStoreApi_v1 {
         Result create(KvPair) = 1;
         Result remove(LongString) = 2;
         Result set(KvPair) = 3;
         Result get(LongString) = 4;
         Result list(LongString) = 5;
      } = 1;
   } = 0x40048086;
}
