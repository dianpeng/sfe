#ifndef ADT_H_
#define ADT_H_

#include <dinject/dinject.h>
#include <cstdint>
#include <cassert>

#include "random.h"


namespace sfe {

template< typename T >
struct Range {
  T lower;
  T upper;
  Range() : lower() , upper () {}
  Range( const T& l , const T& u ) : lower(l) , upper(u) { assert(lower <= upper); }

  // Get a random value inside of the range
  T GetRandom() { return Random::GetInstance().Get(lower,upper); }

 public:
  // DINJECT
  void SetLower( T l ) { lower = l; }
  void SetUpper( T u ) { upper = u; }
};

typedef Range<std::int32_t> IntRange;
typedef Range<float>        FloatRange;
typedef Range<double>       DoubleRange;

// We cannot use sfml::Rect directly since it doesn't support DINJECT
template< typename T >
struct Rect {
  T x , y;
  T width;
  T height;

  Rect() : x() , y() , width() , height() {}
  Rect( T _x , T _y , T w , T h ): x(_x), y(_y), width(w), height(h) {}

 public:
  // DINJECT
  void SetX( T v )      { x = v; }
  void SetY( T v )      { y = v; }
  void SetWidth( T v )  { width = v; }
  void SetHeight( T v ) { height = v; }
};

struct Color {
  std::uint8_t a,r,g,b;
  Color() : a(), r(), g(), b() {}
  Color( std::uint8_t _a ,
         std::uint8_t _r ,
         std::uint8_t _g ,
         std::uint8_t _b ): a(_a), r(_r), g(_g), b(_b) {}
 public:
  // DINJECT
  void SetA( std::uint8_t v ) { a = v; }
  void SetR( std::uint8_t v ) { r = v; }
  void SetG( std::uint8_t v ) { g = v; }
  void SetB( std::uint8_t v ) { b = v; }
};

typedef Rect<std::int32_t> IntRect;
typedef Rect<float>        FloatRect;
typedef Rect<double>       DoubleRect;

} // namespace sfe

#endif // ADT_H_
