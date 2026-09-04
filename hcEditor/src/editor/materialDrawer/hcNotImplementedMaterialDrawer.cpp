#include "hc/editor/materialDrawer/hcNotImplementedMaterialDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  NotImplementedMaterialDrawer::NotImplementedMaterialDrawer()
  {
  }

  NotImplementedMaterialDrawer::~NotImplementedMaterialDrawer()
  {
  }

  materialType::Type NotImplementedMaterialDrawer::getMaterialType() const
  {
    return materialType::Unknown;
  }

  void NotImplementedMaterialDrawer::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    String shadingTypeStr = materialType::toString(material->getMaterialType());
    ImGui::Text("No drawer implemented for this material type %s.", shadingTypeStr.c_str());
  }
}
