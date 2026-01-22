#include "hc/editor/hcUnlitMaterialDescriptorEditor.h"
#include <hc/hcUnlitMaterialDescriptor.h>
#include <hc/hcJsonSerializer.h>
#include <hc/hcLogService.h>
#include <hc/hcFileUtilities.h>
#include "hc/editor/hcProjectFileSelectorView.h"
#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  UnlitMaterialDescriptorEditor::UnlitMaterialDescriptorEditor()
    : m_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_mainImagePath(),
    m_projectFileSelector(nullptr)
  {
  }

  UnlitMaterialDescriptorEditor::~UnlitMaterialDescriptorEditor()
  {
  }

  void UnlitMaterialDescriptorEditor::init(
    SharedPtr<ProjectFileSelectorView> projectFileSelector
  )
  {
    m_projectFileSelector = projectFileSelector;
  }

  void UnlitMaterialDescriptorEditor::copyValuesFrom(
    SharedPtr<MaterialDescriptor> asset
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
    imguiUtilities::DrawColorEdit3("Color", m_color);

    ImGui::Text("Main Image Path: %s", m_mainImagePath.c_str());
    if (ImGui::Button("Select Main Image"))
    {
      if (!m_projectFileSelector)
        return;

      m_projectFileSelector->openImageFileSelector(
        [this](const Path& selectedPath)
        {
          m_mainImagePath = selectedPath;
        }
      );
    }
  }

  void UnlitMaterialDescriptorEditor::save(const Path& path)
  {
    auto unlitDesc = MakeUnique<UnlitMaterialDescriptor>();
    unlitDesc->setColor(m_color);
    unlitDesc->setMainImagePath(m_mainImagePath);

    auto serialized = JsonSerializer::Serialize<MaterialDescriptor>(unlitDesc.get());
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
