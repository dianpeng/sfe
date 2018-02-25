#include "script.h"

/**
 * Script ==============================================
 *
 * require "file1"
 * require "file2"
 *
 * var v0 = "xxx";
 * var v1 = 20.0;
 * var v2 = "hello world";
 *
 * class Base {
 *   MyHellow = 100;
 * }
 *
 * class MyObject(Height,Value) extends Base {
 *  "MyObject" = 10;
 *  "MyObject" = 20;
 *  "Another"  = MyObject1(Height,Value);
 * }
 *
 * object "Particle1" MyObject(Height=20,Value=true );
 *
 * object "Particle2" MyObject(Height=20,Value=false);
 *
 * object "Particle3" MyObject(Height=30,Value="xxx");
 *
 * object "Particle4" MyObject(Height=40,Value="xxx");
 *
 */

namespace sfe {
namespace script {
namespace {

#define SCRIPT_TOKEN_LIST(__)
  __(TK_ADD,"+")                   \
  __(TK_SUB,"-")                   \
  __(TK_MUL,"*")                   \
  __(TK_DIV,"/")                   \
  __(TK_MOD,"%")                   \
  __(TK_POW,"^")                   \
  __(TK_LT ,"<")                   \
  __(TK_LE ,"<=")                  \
  __(TK_GT ,">")                   \
  __(TK_GE ,">=")                  \
  __(TK_EQ ,"==")                  \
  __(TK_NE ,"!=")                  \
  __(TK_QUESTION,"?")              \
  __(TK_COLON,":")                 \
  __(TK_REQUIRE,"require")         \
  __(TK_VAR,"var")                 \
  __(TK_CLASS,"class")             \
  __(TK_OBJECT,"object")           \
  __(TK_EXTENDS,"extends")         \
  __(TK_LPAR,"(")                  \
  __(TK_RPAR,")")                  \
  __(TK_LBRA,"{")                  \
  __(TK_RBRA,"}")                  \
  __(TK_LSQR,"[")                  \
  __(TK_RSQR,"]")                  \
  __(TK_COMMA,",")                 \
  __(TK_SEMICOLON,";")             \
  __(TK_ASSIGN,"=")                \
  __(TK_STRING,"<string>")         \
  __(TK_INT,"<int>")               \
  __(TK_REAL,"<real>")             \
  __(TK_TRUE,"<true>")             \
  __(TK_FALSE,"<false>")           \
  __(TK_IDENTIFIER,"<id>")         \
  __(TK_ERROR,"<error>")           \
  __(TK_EOF,"<eof>")

class Tokenizer {
 public:
  enum {

#define __(A,B) A,
    SCRIPT_TOKEN_LIST(__)
#undef __ // __

    SIZE_OF_TOKENS
  };

  static const char* GetTokenName( int );

  struct Lexeme {
    int          token;
    std::size_t  length;
    std::string  str;
    bool         boolean;
    double       real;
    std::int64_t integer;
  };

 public:
  Tokenizer( const char* source );

  const Lexeme& Next();

  const Lexeme& lexeme() const { return lexeme_; }
  const char*   source() const { return source_; }
  std::size_t   cursor() const { return cursor_; }

 private:
  inline const Lexeme& GetLexeme( int tk , int l );

  const char* source_;
  std::size_t cursor_;
  Lexeme      lexeme_;
};

inline const Tokenizer::Lexeme&
Tokenizer::GetLexeme( int tk , int l ) {
  lexeme_.token = tk;
  lexeme_.length= l;
  return lexeme_;
}

const char* Tokenizer::GetTokenName( int tk ) {
  switch(tk) {

#define __(A,B)
    SCRIPT_TOKEN_LIST(__)
#undef __ // __

    default: return NULL;
  }
}

const Script::Lexeme& Script::Next() {
  for(;;) {
    int c = source_[cursor_];
    switch(c) {
      case '+': return GetLexeme(TK_ADD,1);
      case '-': return GetLexeme(TK_SUB,1);
      case '*': return GetLexeme(TK_MUL,1);
      case '/': return GetLexeme(TK_DIV,1);
      case '%': return GetLexeme(TK_MOD,1);
      case '^': return GetLexeme(TK_POW,1);
    }
  }
}














