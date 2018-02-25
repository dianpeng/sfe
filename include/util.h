#ifndef SFE_UTIL_H_
#define SFE_UTIL_H_

#include <SFML/Graphics.hpp>

namespace sfe {
namespace util {

// Parse a string representation of sfml BlendMode object
// and convert it back to BlendMode. It is mainly used for
// configuring BlendMode via external file or source of
// information
bool ParseBlendMode( const char* , sf::BlendMode* );

} // namespace util
} // namespace sfe

#endif // SFE_UTIL_H_
