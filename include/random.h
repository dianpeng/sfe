#ifndef RANDOM_H_
#define RANDOM_H_

#include <limits>
#include <random>

namespace sfe {

class Random {
 public:
  static Random& GetInstance();

  template< typename T > T Get    ( const T& , const T& );
  template< typename T > T GetInt ( const T& , const T& );
  template< typename T > T GetReal( const T& , const T& );
 private:
  Random(): rd_(), gen_(rd_()) {}
  std::random_device rd_;
  std::mt19937       gen_;
};

template< typename T >
T Random::GetInt( const T& l , const T& u ) {
  return std::uniform_int_distribution<>(l,u)(gen_);
}

template< typename T >
T Random::GetReal( const T& l , const T& u ) {
  return std::uniform_real_distribution<>(l,u)(gen_);
}

template< typename T >
T Random::Get( const T& l , const T& u ) {
  return std::numeric_limits<T>::is_integer ? GetInt(l,u):
                                              GetReal(l,u);
}

} // namespace sfe

#endif // RANDOM_H_
