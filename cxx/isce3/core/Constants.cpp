#include "Constants.h"
#include <algorithm>
#include <isce3/except/Error.h>
#include <map>

using isce3::except::InvalidArgument;

namespace isce3 { namespace core {

dataInterpMethod
parseDataInterpMethod(const std::string & method)
{
    std::string m = method;
    std::transform(m.begin(), m.end(), m.begin(),
        [](unsigned char c) { return std::tolower(c); });

    if (m == "sinc")      return SINC_METHOD;
    if (m == "bilinear")  return BILINEAR_METHOD;
    if (m == "bicubic")   return BICUBIC_METHOD;
    if (m == "nearest")   return NEAREST_METHOD;
    if (m == "biquintic") return BIQUINTIC_METHOD;

    throw InvalidArgument(ISCE_SRCINFO(), "Unknown interp method");
}

}} // isce3::core
