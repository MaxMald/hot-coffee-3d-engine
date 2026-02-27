#include "hc/assets/hcPrimitiveModelsManager.h"
#include "hc/assets/hcVertex.h"
#include "hc/assets/hcModelSubMesh.h"
#include "hc/assets/hcPrimitiveModelsFactory.h"

namespace hc
{
  SharedPtr<Model> PrimitiveModelsManager::getPrimitive(
    primitiveShapeType::Type primitiveType
  )
  {
    if (hasPrimitive(primitiveType))
      return m_primitiveModels[primitiveType];

    SharedPtr<Model> model;
    switch (primitiveType)
    {
      case primitiveShapeType::Cube:
        model = PrimitiveModelsFactory::createCube();
        break;
      case primitiveShapeType::Cone:
        model = PrimitiveModelsFactory::createCone();
        break;
      case primitiveShapeType::Sphere:
        model = PrimitiveModelsFactory::createSphere();
        break;
      case primitiveShapeType::Cylinder:
        model = PrimitiveModelsFactory::createCylinder();
        break;
      case primitiveShapeType::Plane:
        model = PrimitiveModelsFactory::createPlane();
        break;
      case primitiveShapeType::Pyramid:
        model = PrimitiveModelsFactory::createPyramid();
        break;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented primitive shape type: %s",
            primitiveShapeType::toString(primitiveType).c_str()
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
    primitiveShapeType::Type primitiveType
  ) const
  {
    return m_primitiveModels.find(primitiveType) != m_primitiveModels.end();
  }
}
