#include "hc/assets/hcPrimitiveModelPathUtilities.h"

namespace hc
{
  bool PrimitiveModelPathUtilities::IsPrimitiveModelPath(const Path& path)
  {
    String pathStr = path.string();
    return pathStr.find(PrimitivesBasePath) == 0;
  }

  primitiveShapeType::Type PrimitiveModelPathUtilities::GetPrimitiveShapeTypeFromPath(
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
    return primitiveShapeType::fromString(fileName);
  }

  Path PrimitiveModelPathUtilities::GetPrimitiveModelPath(
    primitiveShapeType::Type primitiveType
  )
  {
    String fileName = primitiveShapeType::toString(primitiveType);
    return Path(PrimitivesBasePath) / (fileName + ".hcmodel");
  }
}
