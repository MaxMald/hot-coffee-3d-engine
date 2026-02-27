#include "hc/assets/model/hcPrimitiveModelsManager.h"
#include "hc/assets/model/hcVertex.h"
#include "hc/assets/model/hcModelSubMesh.h"
#include "hc/assets/model/hcPrimitiveModelsFactory.h"

namespace hc
{
  PrimitiveModelsManager::PrimitiveModelsManager(
    IMaterialDescriptorAssetManager& materialDescriptorAssetManager
  ) : m_primitiveModelsFactory(materialDescriptorAssetManager)
  {
  }

  SharedPtr<Model> PrimitiveModelsManager::getPrimitive(
    primitiveModelType::Type primitiveType
  )
  {
    if (hasPrimitive(primitiveType))
      return m_primitiveModels[primitiveType];

    SharedPtr<Model> model;
    switch (primitiveType)
    {
    case primitiveModelType::Undefined:
      throw RuntimeErrorException(
        "Cannot create model for Undefined primitive model type."
      );
    case primitiveModelType::Cube:
      model = m_primitiveModelsFactory.createCube();
      break;
    case primitiveModelType::Cone:
      model = m_primitiveModelsFactory.createCone();
      break;
    case primitiveModelType::Sphere:
      model = m_primitiveModelsFactory.createSphere();
      break;
    case primitiveModelType::Cylinder:
      model = m_primitiveModelsFactory.createCylinder();
      break;
    case primitiveModelType::Plane:
      model = m_primitiveModelsFactory.createPlane();
      break;
    case primitiveModelType::Pyramid:
      model = m_primitiveModelsFactory.createPyramid();
      break;
    default:
      throw RuntimeErrorException(
        String::Format("Not implemented primitive model type: %s",
          primitiveModelType::toString(primitiveType).c_str()
        )
      );
    }

    m_primitiveModels[primitiveType] = model;
    return model;
  }

  void PrimitiveModelsManager::destroy()
  {
    m_primitiveModels.clear();
  }

  bool PrimitiveModelsManager::hasPrimitive(
    primitiveModelType::Type primitiveType
  ) const
  {
    return m_primitiveModels.find(primitiveType) != m_primitiveModels.end();
  }
}
