#ifndef SFE_SCRIPT_H_
#define SFE_SCRIPT_H_
#include <dinject/dinject.h>

namespace sfe {
namespace script {

typedef dinject::ConfigValue  Value;
typedef dinject::ConfigObject Object;

class Scope {
 public:
  Scope( scope* parent ) : parent_(parent) {}
  Scope* parent() const { return parent_; }
  inline bool GetVar( const char* , Value* ) const;
  inline void SetVar( const char* , const Value& );
 private:
  std::map<std::string,Value> var_;
  Scope* parent_;
};

class Script {
 public:
  static std::unique_ptr<Script> ParseFromFile( const char* , const Scope& );
  static std::unique_ptr<Script> ParseFromData( const char* , const Scope& );

 public:
  bool GetVar( const char* , std::int64_t*  ) const;
  bool GetVar( const char* , std::uint64_t* ) const;
  bool GetVar( const char* , std::int32_t*  ) const;
  bool GetVar( const char* , std::uint32_t* ) const;
  bool GetVar( const char* , double* ) const;
  bool GetVar( const char* , float*  ) const;
  bool GetVar( const char* , bool*   ) const;
  bool GetVar( const char* , std::string* ) const;
  bool GetVar( const char* , Value*  ) const;

  bool HasVar( const char* ) const;
  std::shared_ptr<Object> GetObject( const char* ) const;

 private:
  std::map<std::string,Value> var_;
  std::map<std::string,std::shared_ptr<Object>> object_;
};

inline bool Scope::GetVar( const char* name , Value* value ) const {
  auto itr = var_.find(name);
  if(itr != var_.end()) {
    *value = itr->second;
    return true;
  }
  return false;
}

inline void Scope::SetVar( const char* name , const Value& value ) {
  var_[name] = value;
}

} // namespace sfe

#endif // SFE_SCRIPT_H_
