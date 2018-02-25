#ifndef MISC_H_
#define MISC_H_

#define DISALLOW_COPY_AND_ASSIGN(X)    \
  void operator = ( const X& ) = delete; \
  X( const X& ) = delete;

namespace sfe {

void Fatal( const char* msg , ... );

#define fatal_if(COND,FORMAT,...)           \
  do {                                      \
    if(!(COND)) {                           \
      Fatal(FORMAT,__VA_ARGS__);            \
    }                                       \
  } while(false)

#define fatal Fatal

} // namespace sfe

#endif // MISC_H_
