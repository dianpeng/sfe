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

DINJECT_CLASS(Color) {
  dinject::Class<Color>("adt.Color")
    .AddPrimitive("A",&Color::SetA)
    .AddPrimitive("R",&Color::SetR)
    .AddPrimitive("G",&Color::SetG)
    .AddPrimitive("B",&Color::SetB);
}

} // namespace sfe
