#include "hc/assets/model/hcPrimitiveModelPathUtilities.h"

namespace hc
{
  bool PrimitiveModelPathUtilities::IsPrimitiveModelPath(const Path& path)
  {
    String pathStr = path.string();
    return pathStr.find(PrimitivesBasePath) == 0;
  }

  primitiveModelType::Type PrimitiveModelPathUtilities::GetPrimitiveModelTypeFromPath(
    const Path& path
  )
  {
    if (!IsPrimitiveModelPath(path))
    {
      throw RuntimeErrorException(
        String::Format("Path is not a primitive model path: %s", path.string().c_str())
      );
    }

    String fileName = path.stem().string();
    return primitiveModelType::fromString(fileName);
  }

  Path PrimitiveModelPathUtilities::GetPrimitiveModelPath(
    primitiveModelType::Type primitiveType
  )
  {
    String fileName = primitiveModelType::toString(primitiveType);
    return Path(PrimitivesBasePath) / (fileName + ".hcmodel");
  }
}
