#include "hc/editor/services/materialDrawer/hcNotImplementedMaterialDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  NotImplementedMaterialDrawer::NotImplementedMaterialDrawer()
  {}

  NotImplementedMaterialDrawer::~NotImplementedMaterialDrawer()
  {}

  void NotImplementedMaterialDrawer::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    String materialTypeStr = materialType::toString(material->getMaterialType());
    ImGui::Text("No drawer implemented for this material type %s.", materialTypeStr.c_str());
  }

  void NotImplementedMaterialDrawer::drawMeshMaterial(
    IMaterial* material,
    Int32 materialSlotIndex
  )
  {
    if (!material)
      return;

    String materialTypeStr = materialType::toString(material->getMaterialType());
    ImGui::Text(
      "No drawer implemented for this material type %s at slot %d.",
      materialTypeStr.c_str(),
      materialSlotIndex
    );
  }
}
