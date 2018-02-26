#ifndef SFE_CONFIG_H_
#define SFE_CONFIG_H_
#include <dinject/dinject.h>

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

namespace sfe    {
namespace config {

typedef dinject::ConfigValue  Value;
typedef dinject::ConfigObject Object;

void Print( std::ostream* , const Value& );
void Print( std::ostream* , const Object& );

class Function {
 public:

  virtual bool Invoke( const std::vector<Value>& , Value* , std::string* ) = 0;

  virtual ~Function() {}
};

class Scope {
 public:
  Scope( scope* parent ) : parent_(parent) {}
  Scope* parent() const { return parent_; }

  inline Function* GetFunction( const char* ) const;
  inline void      SetFunction( const char* , Function* ) const;

  inline bool      GetVar( const char* , Value* ) const;
  inline void      SetVar( const char* , const Value& );
 private:
  std::map<std::string,Value> var_;
  std::map<std::string,std::unique_ptr<Function>> func_;

  Scope* parent_;
};

class Config {
 public:
  static std::unique_ptr<Config> ParseFromFile( const char* , const Scope& );
  static std::unique_ptr<Config> ParseFromData( const char* , const Scope& );

 public:
  bool GetVar( const char* , std::int64_t*  ) const;
  bool GetVar( const char* , std::uint64_t* ) const;
  bool GetVar( const char* , std::int32_t*  ) const;
  bool GetVar( const char* , std::uint32_t* ) const;
  bool GetVar( const char* , double*        ) const;
  bool GetVar( const char* , float*         ) const;
  bool GetVar( const char* , bool*          ) const;
  bool GetVar( const char* , std::string*   ) const;
  bool GetVar( const char* , Value*         ) const;

  bool HasVar( const char* ) const;
  std::shared_ptr<Object> GetObject( const char* ) const;

 public:
  void Dump( std::ostream* ) const;

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

inline void Scope::SetFunction( const char* name , Function* func ) {
  var_[name].reset(func);
}

inline Function* Scope::GetFunction( const char* name ) const {
  auto itr = func_.find(name);
  if(itr != func_.end()) {
    return itr->second.get();
  }
  return NULL;
}

} // namespace config;
} // namespace sfe

#endif // SFE_CONFIG_H_
