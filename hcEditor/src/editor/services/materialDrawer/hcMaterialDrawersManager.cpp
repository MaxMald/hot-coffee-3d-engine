#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"

#include <imgui.h>

#include "hc/editor/hcEditorCommons.h"
#include "hc/editor/services/materialDrawer/hcIMaterialDrawer.h"
#include "hc/editor/services/materialDrawer/hcUnlitMaterialDrawer.h"
#include "hc/editor/services/materialDrawer/hcBlinnPhongMaterialDrawer.h"
#include "hc/editor/services/materialDrawer/hcHairMaterialDrawer.h"

namespace hc::editor
{
  MaterialDrawersManager::MaterialDrawersManager() :
    m_notImplementedDrawer()
  {}

  void MaterialDrawersManager::prepare()
  {
    addDrawer(MakeUnique<UnlitMaterialDrawer>());
    addDrawer(MakeUnique<BlinnPhongMaterialDrawer>());
    addDrawer(MakeUnique<HairMaterialDrawer>());
  }

  void MaterialDrawersManager::destroy()
  {
    m_drawers.clear();
  }

  void MaterialDrawersManager::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    materialType::Type materialType = material->getMaterialType();
    auto it = m_drawers.find(materialType);
    if (it != m_drawers.end())
      it->second->drawMaterial(material);
    else
      m_notImplementedDrawer.drawMaterial(material);
  }

  void MaterialDrawersManager::drawMeshMaterial(IMaterial* material, Int32 slotIndex)
  {
    if (!material)
      return;

    ImGui::Text("Material: %s", material->getName().c_str());
    ImGui::Separator();
    ImGui::Text("Material Slot: %d", slotIndex);
    ImGui::Text("Material UUID: %s", material->getUUID().toString().c_str());
    ImGui::Text("Material ID: %u", material->getMaterialId());
    ImGui::Text("Material Type: %s", materialType::toString(material->getMaterialType()).c_str());

    // Common properties
    Int32 currentItem = static_cast<Int32>(material->getRenderMode());
    if (ImGui::Combo("Render Mode", &currentItem, comboItems::RENDER_MODE, comboItems::RENDER_MODE_COUNT))
      material->setRenderMode(static_cast<materialRenderMode::Type>(currentItem));

    bool isDoubleSided = material->isDoubleSided();
    if (ImGui::Checkbox("Double Sided", &isDoubleSided))
      material->setDoubleSided(isDoubleSided);

    if (material->getRenderMode() == materialRenderMode::Type::AlphaCutout)
    {
      float alphaCutoff = material->getAlphaCutoutThreshold();
      if (ImGui::SliderFloat("Alpha Cutoff Threshold", &alphaCutoff, 0.0f, 1.0f))
        material->setAlphaCutoutThreshold(alphaCutoff);
    }

    // Concrete properties
    materialType::Type materialType = material->getMaterialType();
    auto it = m_drawers.find(materialType);
    if (it != m_drawers.end())
      it->second->drawMeshMaterial(material, slotIndex);
    else
      m_notImplementedDrawer.drawMeshMaterial(material, slotIndex);
  }

  void MaterialDrawersManager::addDrawer(UniquePtr<IMaterialDrawer> materialDrawer)
  {
    if (!materialDrawer)
      return;

    const auto shadingType = materialDrawer->getMaterialType();
    m_drawers[shadingType] = std::move(materialDrawer);
  }
}
