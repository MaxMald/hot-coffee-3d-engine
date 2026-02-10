#include "hc/hcMaterialFactoriesManager.h"
#include "hc/hcIMaterialFactory.h"

namespace hc
{
  MaterialFactoriesManager::MaterialFactoriesManager()
  {
  }

  MaterialFactoriesManager::~MaterialFactoriesManager()
  {
  }

  void MaterialFactoriesManager::registerFactory(
    shadingType::Type shaderType,
    UniquePtr<IMaterialFactory> factory
  )
  {
    m_factoriesByShaderType[shaderType] = std::move(factory);
  }

  bool MaterialFactoriesManager::hasFactory(shadingType::Type shaderType) const
  {
    return m_factoriesByShaderType.find(shaderType) != m_factoriesByShaderType.end();
  }

  IMaterialFactory& MaterialFactoriesManager::getFactory(
    shadingType::Type shaderType
  ) const
  {
    auto it = m_factoriesByShaderType.find(shaderType);
    if (it == m_factoriesByShaderType.end())
    {
      String shaderTypeStr = shadingType::toString(shaderType);
      throw OutOfRangeException(
        String::Format(
          "No material factory registered for shader type '%s'.",
          shaderTypeStr.c_str()
        )
      );
    }

    if (!it->second)
    {
      String shaderTypeStr = shadingType::toString(shaderType);
      throw RuntimeErrorException(
        String::Format(
          "Material factory for shader type '%s' is null.",
          shaderTypeStr.c_str()
        )
      );
    }

    return *(it->second);
  }
}
