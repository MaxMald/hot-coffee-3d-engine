#include "hc/assets/model/hcPrimitiveModelPathUtilities.h"

namespace hc
{
  static constexpr const char* PRIMITIVE_FOLDER_PATH = "hcPrimitives";

  primitiveModelType::Type PrimitiveModelPathUtilities::GetPrimitiveModelTypeFromPath(
    const Path& path
  )
  {
    if (path.getType() != pathType::Internal)
      throw RuntimeErrorException(
        String::Format("Path is not a primitive model path: %s", path.toString().c_str())
      );

    String fileName = path.stem().toString();
    return primitiveModelType::fromString(fileName);
  }

  Path PrimitiveModelPathUtilities::GetPrimitiveModelPath(
    primitiveModelType::Type primitiveType
  )
  {
    String fileName = primitiveModelType::toString(primitiveType);
    String pathStr= String::Format("%s/%s%s",
      PRIMITIVE_FOLDER_PATH,
      fileName,
      hc::serialization::fileFormat::ModelDescriptor::FILE_EXTENSION
    );

    return Path(pathStr, pathType::Internal);
  }
}
