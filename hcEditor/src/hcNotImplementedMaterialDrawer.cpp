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

  shadingType::Type NotImplementedMaterialDrawer::getShadingType() const
  {
    return shadingType::Unknown;
  }

  void NotImplementedMaterialDrawer::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    String shadingTypeStr = shadingType::toString(material->getShaderType());
    ImGui::Text("No drawer implemented for this material type %s.", shadingTypeStr.c_str());
  }
}
