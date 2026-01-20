#include "hc/hcJsonParsers.h"
#include "hc/hcColor.h"

namespace hc
{
  namespace jsonParsers
  {
    Color parseColor(const Json& json)
    {
      float r = json["r"].getFloat();
      float g = json["g"].getFloat();
      float b = json["b"].getFloat();
      float a = json["a"].getFloat();
      return Color(r, g, b, a);
    }
  }
}
