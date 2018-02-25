#include "random.h"

namespace sfe {

Random& Random::GetInstance() {
  static Random kInstance;
  return kInstance;
}

} // namespace sfe
