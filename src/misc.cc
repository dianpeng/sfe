#include "misc.h"

#include <iostream>
#include <cstdarg>
#include <cstring>
#include <cstdlib>

namespace sfe {

void Fatal( const char* format , ... ) {
  char buf[1024];
  va_list vl;
  va_start(vl,format);

  std::vsprintf(buf,format,vl);
  std::cerr << "Fatal:" << buf << std::endl;
  std::abort();
}

} // namespace sfe
