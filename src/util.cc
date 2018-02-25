#include "util.h"
#include <cctype>
#include <cassert>


namespace sfe {
namespace util{
namespace {

class BlendModeTokenizer {
 public:
  enum {
    // ORDER matters
    // Attribute
    TK_ATTR_SOURCE_COLOR = 0,
    TK_ATTR_SOURCE_ALPHA,
    TK_ATTR_DEST_COLOR  ,
    TK_ATTR_DEST_ALPHA  ,
    TK_ATTR_COLOR_EQUATION,
    TK_ATTR_ALPHA_EQUATION,

    // Builting factor
    TK_ZERO,
    TK_ONE,
    TK_SOURCE_COLOR,
    TK_DEST_COLOR,
    TK_ONE_MINUS_SOURCE_COLOR,
    TK_ONE_MINUS_DEST_COLOR,
    TK_SOURCE_ALPHA,
    TK_DEST_ALPHA,
    TK_ONE_MINUS_SOURCE_ALPHA,
    TK_ONE_MINUS_DEST_ALPHA,

    // Equation
    TK_ADD,
    TK_SUB,

    // Builtin blending mode
    TK_ALPHA_BLENDING,
    TK_ADDITIVE_BLENDING,
    TK_MULTIPLICATIVE_BLENDING,
    TK_NO_BLENDING,

    // Misc tokens
    TK_ASSIGN,
    TK_SEMICOLON,
    TK_ERROR,
    TK_EOF
  };

  static inline bool IsSourceColor( int );
  static inline bool IsDestColor  ( int );
  static inline bool IsSourceAlpha( int );
  static inline bool IsDestAlpha  ( int );
  static bool IsEquation   ( int tk ) { return tk == TK_ADD || tk == TK_SUB; }
  static sf::BlendMode::Factor MapFactor( int );

  int Next();

  BlendModeTokenizer( const char* source ): source_(source), cursor_(0) {}

 private:
  template< std::size_t N > bool Match( std::size_t , const char (&RHS)[N] );

  const char* source_;
  std::size_t cursor_;
};

template< std::size_t N >
bool BlendModeTokenizer::Match( std::size_t offset , const char (&RHS)[N] ) {
  const char* LHS = source_ + cursor_ + offset;
  for( std::size_t i = 0 ; i < N - 1 ; ++i ) {
    if(LHS[i] != RHS[i]) return false;
  }

  if(!std::isdigit(LHS[N])) {
    cursor_ += (offset + (N-1));
    return true;
  }
  return false;
}

int BlendModeTokenizer::Next() {
repeat:
  auto c = source_[cursor_];
  switch(c) {
    case ' ': case '\r': case '\b': case '\n': case '\t': case '\v':
      ++cursor_; goto repeat;
    case '=': ++cursor_; return TK_ASSIGN;
    case ';': ++cursor_; return TK_SEMICOLON;
    case  0 :            return TK_EOF;

    case '.':
      {
        if(Match(1,"src-color"))      return TK_ATTR_SOURCE_COLOR;
        if(Match(1,"src-alpha"))      return TK_ATTR_SOURCE_ALPHA;
        if(Match(1,"dest-color"))     return TK_ATTR_DEST_COLOR;
        if(Match(1,"dest-alpha"))     return TK_ATTR_DEST_ALPHA;
        if(Match(1,"color-equation")) return TK_ATTR_COLOR_EQUATION;
        if(Match(1,"alpha-equation")) return TK_ATTR_ALPHA_EQUATION;
      }
      break;
    case 'o':
      if(Match(0,"one-minus-src-color")) return TK_ONE_MINUS_SOURCE_COLOR;
      if(Match(0,"one-minus-src-alpha")) return TK_ONE_MINUS_SOURCE_ALPHA;
      if(Match(0,"one-minus-dest-color"))return TK_ONE_MINUS_DEST_COLOR;
      if(Match(0,"one-minus-dest-alpha"))return TK_ONE_MINUS_DEST_ALPHA;
      if(Match(0,"one"))                 return TK_ONE;
      break;
    case 's':
      if(Match(0,"src-color")) return TK_SOURCE_COLOR;
      if(Match(0,"src-alpha")) return TK_SOURCE_ALPHA;
      if(Match(0,"sub"))       return TK_SUB;
      break;
    case 'd':
      if(Match(0,"dest-color")) return TK_DEST_COLOR;
      if(Match(0,"dest-alpha")) return TK_DEST_ALPHA;
      break;
    case 'z':
      if(Match(0,"zero"))       return TK_ZERO;
      break;
    case 'a':
      if(Match(0,"alpha-blending"))    return TK_ALPHA_BLENDING;
      if(Match(0,"additive-blending")) return TK_ADDITIVE_BLENDING;
      if(Match(0,"add"))               return TK_ADD;
      break;
    case 'm':
      if(Match(0,"multiplicative-blending")) return TK_MULTIPLICATIVE_BLENDING;
      break;
    case 'n':
      if(Match(0,"no-blending")) return TK_NO_BLENDING;
      break;
    default:
      break;
  }

  return TK_ERROR;
}

inline bool BlendModeTokenizer::IsSourceColor( int t ) {
  switch(t) {
    case TK_ZERO:
    case TK_ONE:
    case TK_SOURCE_COLOR:
    case TK_ONE_MINUS_SOURCE_COLOR:
      return true;
    default:
      return false;
  }
}

inline bool BlendModeTokenizer::IsSourceAlpha( int t ) {
  switch(t) {
    case TK_ZERO:
    case TK_ONE:
    case TK_SOURCE_ALPHA:
    case TK_ONE_MINUS_SOURCE_ALPHA:
      return true;
    default:
      return false;
  }
}

inline bool BlendModeTokenizer::IsDestColor( int t ) {
  switch(t) {
    case TK_ZERO:
    case TK_ONE:
    case TK_DEST_COLOR:
    case TK_ONE_MINUS_DEST_COLOR:
      return true;
    default:
      return false;
  }
}

inline bool BlendModeTokenizer::IsDestAlpha( int t ) {
  switch(t) {
    case TK_ZERO:
    case TK_ONE:
    case TK_DEST_ALPHA:
    case TK_ONE_MINUS_DEST_ALPHA:
      return true;
    default:
      return false;
  }
}

inline sf::BlendMode::Factor BlendModeTokenizer::MapFactor( int tk ) {
  switch(tk) {
    case TK_ZERO: return sf::BlendMode::Zero;
    case TK_ONE:  return sf::BlendMode::One ;
    case TK_SOURCE_COLOR: return sf::BlendMode::SrcColor;
    case TK_SOURCE_ALPHA: return sf::BlendMode::SrcAlpha;
    case TK_DEST_COLOR:   return sf::BlendMode::DstColor;
    case TK_DEST_ALPHA:   return sf::BlendMode::DstAlpha;
    case TK_ONE_MINUS_SOURCE_COLOR: return sf::BlendMode::OneMinusSrcColor;
    case TK_ONE_MINUS_SOURCE_ALPHA: return sf::BlendMode::OneMinusSrcAlpha;
    case TK_ONE_MINUS_DEST_ALPHA:   return sf::BlendMode::OneMinusDstAlpha;
    case TK_ONE_MINUS_DEST_COLOR:   return sf::BlendMode::OneMinusDstColor;
    default: assert(false); return sf::BlendMode::Zero;
  }
}

} // namespace

bool ParseBlendMode( const char* source , sf::BlendMode* output ) {
  BlendModeTokenizer t(source);
  auto tk = t.Next();

  // try to check whether it is predefined blending mode
  switch(tk) {
    case BlendModeTokenizer::TK_ALPHA_BLENDING:
      *output = sf::BlendAlpha; return true;
    case BlendModeTokenizer::TK_ADDITIVE_BLENDING:
      *output = sf::BlendAdd;   return true;
    case BlendModeTokenizer::TK_MULTIPLICATIVE_BLENDING:
      *output = sf::BlendMultiply; return true;
    case BlendModeTokenizer::TK_NO_BLENDING:
      *output = sf::BlendNone;    return true;
    default: break;
  }

  bool flags[6] = {false};

  do {
    int key;

    switch(tk) {
      case BlendModeTokenizer::TK_ATTR_SOURCE_COLOR:
        if(flags[tk]) return false;
        break;
      case BlendModeTokenizer::TK_ATTR_SOURCE_ALPHA:
        if(flags[tk]) return false;
        break;
      case BlendModeTokenizer::TK_ATTR_DEST_COLOR:
        if(flags[tk]) return false;
        break;
      case BlendModeTokenizer::TK_ATTR_DEST_ALPHA:
        if(flags[tk]) return false;
        break;
      case BlendModeTokenizer::TK_ATTR_ALPHA_EQUATION:
        if(flags[tk]) return false;
        break;
      case BlendModeTokenizer::TK_ATTR_COLOR_EQUATION:
        if(flags[tk]) return false;
        break;
      default: return false;
    }

    key = tk;
    flags[tk] = true;

    if(t.Next() != BlendModeTokenizer::TK_ASSIGN)
      return false;

    int val = t.Next();
    switch(val) {
      case BlendModeTokenizer::TK_ZERO:
      case BlendModeTokenizer::TK_ONE:
      case BlendModeTokenizer::TK_ONE_MINUS_DEST_ALPHA:
      case BlendModeTokenizer::TK_ONE_MINUS_DEST_COLOR:
      case BlendModeTokenizer::TK_ONE_MINUS_SOURCE_COLOR:
      case BlendModeTokenizer::TK_ONE_MINUS_SOURCE_ALPHA:
      case BlendModeTokenizer::TK_SOURCE_ALPHA:
      case BlendModeTokenizer::TK_SOURCE_COLOR:
      case BlendModeTokenizer::TK_DEST_ALPHA:
      case BlendModeTokenizer::TK_DEST_COLOR:
      case BlendModeTokenizer::TK_ADD:
      case BlendModeTokenizer::TK_SUB:
        break;
      default: return false;
    }

    switch(key) {
      case BlendModeTokenizer::TK_ATTR_SOURCE_COLOR:
        if(!BlendModeTokenizer::IsSourceColor(val)) return false;
        output->colorSrcFactor = BlendModeTokenizer::MapFactor(val);
        break;
      case BlendModeTokenizer::TK_ATTR_SOURCE_ALPHA:
        if(!BlendModeTokenizer::IsSourceAlpha(val)) return false;
        output->alphaSrcFactor = BlendModeTokenizer::MapFactor(val);
        break;
      case BlendModeTokenizer::TK_ATTR_DEST_COLOR:
        if(!BlendModeTokenizer::IsDestColor(val)) return false;
        output->colorDstFactor = BlendModeTokenizer::MapFactor(val);
        break;
      case BlendModeTokenizer::TK_ATTR_DEST_ALPHA:
        if(!BlendModeTokenizer::IsDestAlpha(val)) return false;
        output->alphaDstFactor = BlendModeTokenizer::MapFactor(val);
        break;
      case BlendModeTokenizer::TK_ATTR_COLOR_EQUATION:
        if(!BlendModeTokenizer::IsEquation(val)) return false;
        output->colorEquation =
          (val == BlendModeTokenizer::TK_ADD) ? sf::BlendMode::Add :
                                                sf::BlendMode::Subtract;
        break;
      default:
        if(!BlendModeTokenizer::IsEquation(val)) return false;
        output->alphaEquation =
          (val == BlendModeTokenizer::TK_ADD) ? sf::BlendMode::Add :
                                                sf::BlendMode::Subtract;
        break;
    }

    if((tk=t.Next()) == BlendModeTokenizer::TK_SEMICOLON)
      tk = t.Next(); // skip ; if needed

  } while( !flags[0] || !flags[1] || !flags[2] ||
           !flags[3] || !flags[4] || !flags[5] );

  return true;
}

} // namespace util
} // namespace sfe
