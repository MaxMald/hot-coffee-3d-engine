#include "hc/editor/hcNullMaterialDescriptorEditor.h"
#include "imgui.h"

namespace hc::editor
{
  void NullMaterialDescriptorEditor::init(
    SharedPtr<ProjectFileSelectorView>
  )
  {
  }

  void NullMaterialDescriptorEditor::copyValuesFrom(
    SharedPtr<MaterialDescriptor>
  )
  {
  }

  void NullMaterialDescriptorEditor::draw()
  {
    ImGui::Text("No editor available for this material type.");
  }

  void NullMaterialDescriptorEditor::save(const Path&)
  {
    LogService::Warning(
      "Attempted to save using NullMaterialDescriptorEditor. No action taken."
    );
  }

  void NullMaterialDescriptorEditor::clear()
  {
  }
}
