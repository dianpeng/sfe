#include "config.h"
#include <cstdlib>

namespace sfe {
namespace config {
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
  __(TK_AND,"&&")                  \
  __(TK_OR,"||")                   \
  __(TK_NOT,"!")                   \
  __(TK_QUESTION,"?")              \
  __(TK_COLON,":")                 \
  __(TK_DOT,".")                   \
  __(TK_INCLUDE,"include")         \
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

  static inline bool IsIdInitChar( char );
  static inline bool IsIdRestChar( char );

 public:
  Tokenizer( const char* source );

  const Lexeme& Next();

  const Lexeme& lexeme() const { return lexeme_; }
  const char*   source() const { return source_; }
  std::size_t   cursor() const { return cursor_; }
  std::size_t   line  () const { return line_;   }
  std::size_t   ccount() const { return ccount_; }

 private:
  inline const Lexeme& GetLexeme( int tk , int l );
  inline const Lexeme& Error    ( const char* , ... );
  inline const Lexeme& Predicate( char p , int tk1 , int tk2 );
  inline const Lexeme& Predicate( char p , int tk );

  template< std::size_t >
  bool Match( std::size_t , const char (&RHS)[N] );

  const Lexeme& LexNumber();
  const Lexeme& LexString();
  const Lexeme& LexKeywordOrIdentifier();

  const char* source_;
  std::size_t cursor_;
  std::size_t line_  ;
  std::size_t ccount_;
  Lexeme      lexeme_;
};

inline bool Tokenizer::IsIdInitChar( char c ) {
  return std::isalpha(c) || c == '_';
}

inline bool Tokenizer::IsIdRestChar( char c ) {
  return IsIdInitChar(c) || std::isdigit(c);
}

inline const Tokenizer::Lexeme&
Tokenizer::GetLexeme( int tk , int l ) {
  lexeme_.token = tk;
  lexeme_.length= l;
  cursor_ += l;
  ccount_ += l;
  return lexeme_;
}

inline const Tokenizer::Lexeme&
Tokenizer::Error( const char* format , ... ) {
  lexeme_.str.clear();
  Format(&(lexeme_.str),"error at %zu and %zu:",line_,ccount_);

  va_list vl;
  va_start(vl,format);
  Format(&(lexeme_.str),format,vl);

  lexeme_.token = TK_ERROR;
  return lexeme_;
}

inline const Tokenizer::Lexeme&
Tokenizer::Predicate( char p , int tk1 , int tk2 ) {
  if(source_[cursor_+1] == p)
    return GetLexeme(tk2,2);
  else
    return GetLexeme(tk1,1);
}

inline const Tokenizer::Lexeme&
Tokenizer::Predicate( char p , int tk ) {
  if(source_[cursor_+1] != p)
    return Error("expect character %c for token %s, but get character %c",
        p,GetTokenName(tk),source_[cursor_+1]);

  return GetLexeme(tk,2);
}

const char* Tokenizer::GetTokenName( int tk ) {
  switch(tk) {

#define __(A,B)
    SCRIPT_TOKEN_LIST(__)
#undef __ // __

    default: return NULL;
  }
}

const Tokenizer::Lexeme&
Tokenizer::LexNumber() {
  char* pend = NULL;

  errno = 0;
  double v = strtod(source_ + cursor_,&pend);
  if(errno == ERANGE) {
    // cannot hold inside of double, it might be a large integer number
    errno = 0;
    auto iv = strtoll(source_ + cursor_,&pend,10);
    if(errno == ERANGE) {
      return Error("the number is too large to be held either in double or int64_t!");
    }
    lexeme_.token   = TK_INT;
    lexeme_.integer = static_cast<std::int64_t>(iv);
    lexeme_.length  = (pend - (source_ + cursor_));
  } else {
    lexeme_.token   = TK_REAL;
    lexeme_.real    = v;
    lexeme_.length  = (pend - (source_ + cursor_));
  }

  cursor_ += lexeme_.length;
  ccount_ += lexeme_.length;
  return lexeme_;
}

const Tokenizer::Lexeme& Tokenizer::LexString() {
  char c;
  auto start = cursor_;

  assert( source_[cursor_] == '\"' );
  ++cursor_;

  lexeme_.str.clear();
  for( ; (c = source_[cursor_]) ; ++cursor_ ) {
    if(c == '\\') {
      int nc = source_[cursor_];
      switch(nc) {
        case 'n': ++cursor_; lexeme_.str.push_back('\n'); break;
        case 't': ++cursor_; lexeme_.str.push_back('\t'); break;
        case 'v': ++cursor_; lexeme_.str.push_back('\v'); break;
        case 'r': ++cursor_; lexeme_.str.push_back('\r'); break;
        case 'b': ++cursor_; lexeme_.str.push_back('\b'); break;
        case '\\':++cursor_; lexeme_.str.push_back('\\'); break;
        case '"': ++cursor_; lexeme_.str.push_back('"');  break;
        default: return Error("unknown escaped character %c",nc);
      }
    } else if(c == '"') {
      break;
    } else {
      lexeme_.str.push_back(c);
    }
  }

  ++cursor_;
  lexeme_.length = (cursor_-start);
  lexeme_.token  = TK_STRING;
  ccount_ += lexeme_.length;

  return lexeme_;
}

template< std::size_t N >
bool Tokenizer::Match( std::size_t offset , const char (&RHS)[N] ) {
  const char* LHS = source_ + cursor_ + offset;
  for( std::size_t i = 0 ; i < (N-1) ; ++i ) {
    if(LHS[i] != RHS[i]) return false;
  }
  return !IsIdRestChar(LHS[N]);
}

const Tokenizer::Lexeme&
Tokenizer::LexKeywordOrIdentifier() {
  // var , include , extends, class , object, true, false
  switch(source_[cursor_]) {
    case 'c':
      if(Match(1,"lass"))
        return GetLexeme(TK_CLASS,5);
      break;
    case 'e':
      if(Match(1,"xtends"))
        return GetLexeme(TK_EXTENDS,7);
      break;
    case 'f':
      if(Match(1,"alse"))
        return GetLexeme(TK_FALSE,5);
      break;
    case 'i':
      if(Match(1,"nclude"))
        return GetLexeme(TK_INCLUDE,7);
      break;
    case 'o':
      if(Match(1,"bject"))
        return GetLexeme(TK_OBJECT,6);
      break;
    case 't':
      if(Match(1,"rue"))
        return GetLexeme(TK_TRUE,4);
      break;
    case 'v':
      if(Match(1,"ar"))
        return GetLexeme(TK_VAR,3);
      break;
    default: break;
  }

  // lexing keyword
  if(IsIdInitChar(source_[cursor_])) {
    char c;
    auto start = cursor_;
    lexeme_.str.clear();
    lexeme_.str.push_back(source_[cursor_]);
    for( ++cursor ; (c = source_[cursor_]) ; ++cursor_ ) {
      if(!IsIdRestChar(c)) break;
      lexeme_.str.push_back(c);
    }

    lexeme_.length = (cursor_ - start);
    lexeme_.token  = TK_IDENTIFIER;
    ccount_ += lexeme_.length;
    return lexeme_;
  }

  return Error("unknown character %c",source_[cursor_]);
}

const Tokenizer::Lexeme& Tokenizer::Next() {
  for(;;) {
    int c = source_[cursor_];
    switch(c) {
      case  0 : return GetLexeme(TK_EOF,0);
      case ' ' : case '\t': case '\r':
      case '\v': case '\b':
        ++cursor_; ++ccount_; continue;

      case '\n':
        ++line_; ccount_ = 1; ++cursor_; continue;

      case '+': return GetLexeme(TK_ADD,1);
      case '-': return GetLexeme(TK_SUB,1);
      case '*': return GetLexeme(TK_MUL,1);
      case '/': return GetLexeme(TK_DIV,1);
      case '%': return GetLexeme(TK_MOD,1);
      case '^': return GetLexeme(TK_POW,1);
      case '>': return Predicate('=',TK_GT,TK_GE);
      case '<': return Predicate('=',TK_LT,TK_LE);
      case '=': return Predicate('=',TK_ASSIGN,TK_EQ);
      case '!': return Predicate('=',TK_NOT,TK_NE);
      case '&': return Predicate('&',TK_AND);
      case '|': return Predicate('|',TK_OR );
      case '?': return GetLexeme(TK_QUESTION,1);
      case ':': return GetLexeme(TK_COLON,1);
      case '.': return GetLexeme(TK_DOT,1);
      case ',': return GetLexeme(TK_COMMA,1);
      case ';': return GetLexeme(TK_SEMICOLON,1);
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        return LexNumber();
      case '"':
        return LexString();
      default : return LexKeywordOrIdentifier();
    }
  }
}














