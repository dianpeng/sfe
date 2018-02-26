#ifndef SFE_UTIL_H_
#define SFE_UTIL_H_
#include <SFML/Graphics.hpp>
#include <cstdarg>

namespace sfe {
namespace util {

void Format ( std::string* , const char* , ... );
void FormatV( std::string* , const char* , va_list );

inline std::string Format( const char* format , ... ) {
  std::string buf;
  va_list va;
  va_start(va,format);
  FormatV(&buf,format,vl);
  return buf;
}

inline std::string FormatV( const char* format , va_list vl ) {
  std::string buf;
  FormatV(&buf,format,vl);
  return buf;
}

inline char* AsBuffer( std::string& output , std::size_t offset = 0 ) {
  return &(*(output->begin())) + offset;
}

inline const char* AsBuffer( const std::string& output ,
                             std::size_t offset = 0 ) {
  return &(*(output->begin())) + offset;
}

// Parse a string representation of sfml BlendMode object
// and convert it back to BlendMode. It is mainly used for
// configuring BlendMode via external file or source of
// information
bool ParseBlendMode( const char* , sf::BlendMode* );

} // namespace util
} // namespace sfe

#endif // SFE_UTIL_H_
