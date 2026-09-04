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

    materialType::Type materialType = material->getMaterialType();
    auto it = m_drawers.find(materialType);
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

    const auto shadingType = materialDrawer->getMaterialType();
    m_drawers[shadingType] = std::move(materialDrawer);
  }

  void MaterialDrawersManager::clear()
  {
    m_drawers.clear();
  }
}
