#include "hc/utilities/hcJsonParsers.h"
#include "hc/utilities/hcColor.h"

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

    Path parsePath(const Json& json)
    {
      String pathStr = json["path"].getString();
      Int32 pathTypeInt = json["type"].getInt32();
      return Path(pathStr, static_cast<pathType::Type>(pathTypeInt));
    }
  }
}
