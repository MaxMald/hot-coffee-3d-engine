#include "hc/editor/materialDrawer/hcUnlitMaterialDescriptorEditor.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  UnlitMaterialDescriptorEditor::UnlitMaterialDescriptorEditor(
    ProjectFileDialogView& projectFileSelector
  ) : 
    m_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_mainImagePath(),
    m_projectFileSelector(projectFileSelector)
  {
  }

  UnlitMaterialDescriptorEditor::~UnlitMaterialDescriptorEditor()
  {
  }

  void UnlitMaterialDescriptorEditor::init()
  {
  }

  void UnlitMaterialDescriptorEditor::copyValuesFrom(
    SharedPtr<AMaterialDescriptor> asset
  )
  {
    auto unlitDesc = std::dynamic_pointer_cast<UnlitMaterialDescriptor>(asset);
    if (!unlitDesc)
      return;

    m_color = unlitDesc->getColor();
    m_mainImagePath = unlitDesc->getMainImagePath();
  }

  void UnlitMaterialDescriptorEditor::draw()
  {
    imguiUtilities::DrawColorEdit3("Tint: ", m_color);
    if (ImGui::Button("Select Main Image"))
    {
      m_projectFileSelector.openImageFile(
        [this](const Path& selectedPath)
        {
          m_mainImagePath = selectedPath;
        }
      );
    }

    ImGui::SameLine();
    String mainImagePathStr = m_mainImagePath.string();
    ImGui::Text("Current Main Image Path: %s", mainImagePathStr.c_str());
  }

  void UnlitMaterialDescriptorEditor::save(const Path& path)
  {
    auto unlitDesc = MakeUnique<UnlitMaterialDescriptor>(
      path,
      m_color,
      m_mainImagePath
    );

    auto serialized = JsonSerializer::Serialize<AMaterialDescriptor>(unlitDesc.get());
    if (!serialized.has_value())
    {
      LogService::Error("Failed to serialize UnlitMaterialDescriptor.");
      return;
    }

    if (fileUtilities::SaveStringToFile(path, serialized.value()))
    {
      LogService::Message(
        String::Format(
          "UnlitMaterialDescriptor saved successfully to %s", path.generic_string().c_str()
        )
      );
    }
    else
    {
      LogService::Error(
        String::Format(
          "Failed to save UnlitMaterialDescriptor to %s", path.generic_string().c_str()
        )
      );
    }
  }

  void UnlitMaterialDescriptorEditor::clear()
  {
    m_color = Color(0.0f, 0.0f, 0.0f, 1.0f);
    m_mainImagePath.clear();
  }
}
