#include "adt.h"

namespace sfe {

DINJECT_CLASS(IntRange) {
  dinject::Class<IntRange>("adt.IntRange")
    .AddPrimitive("lower",&IntRange::SetLower)
    .AddPrimitive("upper",&IntRange::SetUpper);
}

DINJECT_CLASS(FloatRange) {
  dinject::Class<FloatRange>("adt.FloatRange")
    .AddPrimitive("lower",&FloatRange::SetLower)
    .AddPrimitive("upper",&FloatRange::SetUpper);
}

DINJECT_CLASS(DoubleRange) {
  dinject::Class<DoubleRange>("adt.DoubleRange")
    .AddPrimitive("lower",&DoubleRange::SetLower)
    .AddPrimitive("upper",&DoubleRange::SetUpper);
}

} // namespace sfe
