#include "hc/editor/materialDrawer/hcMaterialDrawersManager.h"
#include "hc/editor/materialDrawer/hcIMaterialDrawer.h"

namespace hc::editor
{
  MaterialDrawersManager::MaterialDrawersManager() :
    m_notImplementedDrawer()
  {
  }

  void MaterialDrawersManager::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    shadingType::Type shadingType = material->getShaderType();
    auto it = m_drawers.find(shadingType);
    if (it != m_drawers.end())
    {
      it->second->drawMaterial(material);
    }
    else
    {
      m_notImplementedDrawer.drawMaterial(material);
    }
  }

  void MaterialDrawersManager::addDrawer(UniquePtr<IMaterialDrawer> materialDrawer)
  {
    if (!materialDrawer)
      return;

    const auto shadingType = materialDrawer->getShadingType();
    m_drawers[shadingType] = std::move(materialDrawer);
  }

  void MaterialDrawersManager::clear()
  {
    m_drawers.clear();
  }
}
