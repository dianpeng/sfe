#include "config.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace sfe {
namespace config {
namespace {


// ==============================================================
//
// Tokenizer
//
// ==============================================================

#define CONFIG_TOKEN_LIST(__)      \
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
  __(TK_DOLLAR,"$")                \
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
    CONFIG_TOKEN_LIST(__)
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

  template< std::size_t N >
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
    CONFIG_TOKEN_LIST(__)
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
    for( ++cursor_ ; (c = source_[cursor_]) ; ++cursor_ ) {
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
      case '$': return GetLexeme(TK_DOLLAR,1);
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

// ====================================================
//
// AST
//
// ====================================================
namespace ast {

#define CONFIG_AST_LIST(__)            \
  __(INT,Int,"int")                    \
  __(REAL,Real,"real")                 \
  __(BOOLEAN,Boolean,"boolean")        \
  __(STRING,String,"string")           \
  __(DICT,Dict,"dict")                 \
  __(IDENT ,Ident ,"ident")            \
  __(CALL  ,Call,"call")               \
  __(PREFIX,Prefix,"prefix")           \
  __(UNARY,Unary,"unary")              \
  __(BINARY,Binary,"binary")           \
  __(TERNARY,Ternary,"ternary")        \
  __(VAR,Var,"var")                    \
  __(CLASS,Class,"class")              \
  __(OBJ_INST,ObjInst,"objinst")       \
  __(OBJ_INL ,ObjInl ,"objinl" )


#define __(A,B,...) class B;
  CONFIG_AST_LIST(__)
#undef __ // __

enum Type {
#define __(A,...) AST_##A,
  CONFIG_AST_LIST(__)
#undef __ // __
  SIZE_OF_AST
};

struct Node {
  Type type; int line , ccount;
  Node(Type t, int l , int c ): type(t), line(l), ccount(c) {}
  virtual ~Node() {}
};

struct Int : public Node {
  std::int64_t value;
  Int( int l , int c , std::int64_t v ): Node(AST_INT,l,c), value(v) {}
};

struct Real: public Node {
  double value;
  Real( int l , int c , double v ): Node(AST_REAL,l,c), value(v) {}
};

struct Boolean : public Node {
  bool value;
  Boolean( int l , int c , bool v ): Node(AST_BOOLEAN,l,c), value(v) {}
};

struct String : public Node {
  std::string value;
  String( int l , int c , const std::string& v ): Node(AST_STRING,l,c), value(v) {}
};

struct Dict : public Node {
  struct Item { Node* k; Node* v; };
  std::vector<Item> map;
  Dict( int l , int c ) : Node(AST_DICT,l,c), map() {}
};

struct Ident : public Node {
  std::string name;
  Ident( int l , int c ) : Node( AST_IDENT , l, c), name() {}
};

struct Call : public Node {
  std::vector<Node*> arg;
  Call( int l , int c ) : Node(AST_CALL,l,c), arg() {}
};

struct Prefix : public Node {
  struct Comp {
    union { Ident* id; Node* idx; Call* call; };
    enum { DOT , INDEX , CALL };
    int tag;
  };
  std::vector<Comp> list;
  Prefix( int l , int c ) : Node(AST_PREFIX,l,c), list() {}
};

struct Unary : public Node {
  Node* v;
  std::vector<int> op;
  Unary( int l , int c ): Node(AST_UNARY,l,c), v(), op() {}
};

struct Binary : public Node {
  Node* lhs, *rhs;
  int op;
  Binary( int l , int c ): Node(AST_BINARY,l,c), lhs(), rhs(), op() {}
};

struct Ternary : public Node {
  Node* cond; Node* lhs, *rhs;
  Ternary( int l , int c ) : Node(AST_TERNARY,l,c), cond(), lhs(), rhs() {}
};

struct Var : public Node {
  Ident* name;
  Node*  value;
  Var( int l , int c ) : Node(AST_VAR,l,c), name(), value() {}
};

struct Class : public Node {
  std::string name;
  std::vector<std::string base;
  std::vector<std::string> arg;
  Dict*  body;

  Class( int l , int c , std::string&& n , Call* ct , Dict* bd ):
    Node(AST_CLASS,l,c),
    name(n),
    base(),
    arg (),
    body(bd)
  {}
};

struct ObjInst : public Node {
  std::string name;
  std::string class_name;
  std::vector<Node*> arg;

  ObjInst( int l , int c , std::string&& n , std::string&& cn ):
    Node(AST_OBJ_INST,l,c),
    name(n),
    class_name(cn),
    arg()
  {}
};

struct ObjInl : public Node {
  std::string name;
  Node* expr;

  ObjInl( int l , int c , std::string& n , Node* e ):
    Node(AST_OBJ_INL, l, c),
    name(n),
    expr(e)
  {}
};

struct Root: public Node {
  std::vector<Var*>   var;
  std::vector<Class*> cls;
  std::vector<Node*>  obj;

  Root( int l , int c ):
    Node(AST_ROOT,l,c),
    var(),
    cls(),
    obj()
  {}
};

class BumpAllocator {
 public:
  inline BumpAllocator( std::size_t init_capacity ,
                        std::size_t maximum_size  );

  ~BumpAllocator() { Clear(); }

  // Grab memory from BumpAllocator
  void* Grab( std::size_t );
  void* Grab( std::size_t sz , std::size_t alignment ) {
    return Grab( Align(sz,alignment) );
  }
  template< typename T > T* Grab() { return static_cast<T*>(Grab(sizeof(T))); }

 public:
  std::size_t size() const { return size_; }
  std::size_t maximum_size() const { return maximum_size_; }
  void set_maximum_size( std::size_t sz ) { maximum_size_ = sz; }
  std::size_t segment_size() const { return segment_size_; }
  std::size_t current_capacity() const { return current_capacity_; }
  std::size_t total_bytes() const { return total_bytes_; }

 public:
  void Reset();

 private:
  void Clear();
  void RefillPool( std::size_t );

  struct Segment {
    Segment* next;
  };

  Segment* segment_;                       // First segment list
  void* pool_;                             // Starting position of the current pool
  std::size_t init_capacity_;              // Initialized capacity
  std::size_t size_;                       // How many times the Grab has been called
  std::size_t current_capacity_;           // Current capacity
  std::size_t used_;                       // Used size for the current pool
  std::size_t segment_size_;               // Size of all the segment
  std::size_t maximum_size_;               // Maximum size of BumpAllocator
  std::size_t total_bytes_ ;               // How many bytes has been allocated , include the Segment header

  DISALLOW_COPY_AND_ASSIGN(BumpAllocator);
};

inline BumpAllocator::BumpAllocator( std::size_t init_capacity ,
                                     std::size_t maximum_size  ) {
  segment_         (NULL),
  pool_            (NULL),
  init_capacity_   (init_capacity),
  size_            (0),
  current_capacity_(init_capacity),
  used_            (0),
  segment_size_    (0),
  maximum_size_    (maximum_size),
  total_bytes_     (0)
{
  RefillPool(init_capacity);
}

void BumpAllocator::RefillPool( std::size_t size ) {
  const size_t total = size + sizeof(Segment);
  void* ptr = :malloc(total);

  Segment* segment = reinterpret_cast<Segment*>(ptr);
  segment->next = segment_;
  segment_ = segment;

  pool_ = reinterpret_cast<void*>(
      static_cast<char*>(ptr) + sizeof(Segment));

  current_capacity_ = size;
  used_ = 0;
  total_bytes_ += total;
  ++segment_size_;
}

void* BumpAllocator::Grab( std::size_t size ) {
  assert(size);

  if( used_ + size > current_capacity_ ) {
    size_t new_cap = current_capacity_ * 2;
    if(new_cap > maximum_size_) new_cap = maximum_size_;
    if(new_cap < size ) new_cap = size;
    RefillPool(new_cap);
  }

  assert( current_capacity_ - used_ >= size );

  void* ret = pool_;
  pool_ = reinterpret_cast<void*>(static_cast<char*>(pool_) + size);
  used_ += size;
  size_ += size;
  return ret;
}

void BumpAllocator::Clear() {
  while(segment_) {
    Segment* n = segment_->next;
    :free(segment_);
    segment_ = n;
  }
}

void BumpAllocator::Reset() {
  Clear();

  pool_ = NULL;
  size_ = 0;
  current_capacity_ = 0;
  used_ = 0;
  segment_size_ = 0;

  RefillPool(init_capacity_);
}

} // namespace ast


// ========================================================
//
// Parser
//
// ======================================================

class Parser {
 public:
  Parse( const char* );

 private:

 private:
  template< typename T , typename ...ARG >
  T* NewNode( ARG ...args ) {
    void* data = allocator_.Grab(sizeof(T));
    return ::new (data) T(args...);
  }

  struct Unit {
    Tokenizer   tk;
    std::string file;
    Unit( const char* source , const std::string& name ):
      tk(source),
      file(name)
    {}
  };

  std::vector<Unit>  state_;        // parse unit states
  BumpAllocator      allocator;     // allocator
  std::vector<Node*> nodes_;        // used to call destructor
};












