#include "hc/editor/hcMaterialDescriptorEditorWindow.h"
#include <hc/hcUnlitMaterialDescriptor.h>
#include <hc/hcJsonSerializer.h>
#include <hc/hcLogService.h>
#include <hc/hcDependencyContainer.h>
#include <hc/hcFileUtilities.h>
#include "hc/editor/hcImguiUtilities.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelectorView.h"
#include "imgui.h"

namespace
{
  constexpr const hc::Char* SHADER_TYPES[] =
  {
    "Unlit"
  };
}

namespace hc::editor
{
  MaterialDescriptorEditorWindow::MaterialDescriptorEditorWindow() :
    AWindowView("Material Descriptor Editor", false),
    m_materialDescriptor(),
    m_currentShaderType(shaderType::Type::Unknown),
    m_unlitColor(0.0f, 0.0f, 0.0f, 1.0f),
    m_unlitMainImageKey()
  {
  }

  MaterialDescriptorEditorWindow::~MaterialDescriptorEditorWindow()
  {
  }

  void MaterialDescriptorEditorWindow::open(const Path& materialDescriptorPath)
  {
    clear();

    Optional<String> fileContent =
      fileUtilities::LoadStringFromFile(materialDescriptorPath);

    if (!fileContent.has_value())
      return;

    MaterialDescriptor* materialDescriptor =
      JsonSerializer::Deserialize<MaterialDescriptor>(fileContent.value());

    if (!materialDescriptor)
      return;

    m_materialDescriptor.setFilePath(materialDescriptorPath);
    m_materialDescriptor.setAsset(SharedPtr<MaterialDescriptor>(materialDescriptor));
    m_currentShaderType = materialDescriptor->getShaderType();
    updateShaderTypeCombo();

    m_isOpen = true;
  }

  void MaterialDescriptorEditorWindow::resolveDependencies(
    DependencyContainer& container
  )
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_projectFileSelectorView = container.resolve<ProjectFileSelectorView>();
  }

  void MaterialDescriptorEditorWindow::clear()
  {
    m_materialDescriptor = AssetFileReference<MaterialDescriptor>();
    m_currentShaderType = shaderType::Type::Unknown;
  }

  void MaterialDescriptorEditorWindow::onDraw()
  {
    if (!m_materialDescriptor.getAsset())
    {
      ImGui::Text("No material descriptor loaded.");
      return;
    }

    drawShaderTypeSelector();

    if (m_currentShaderType == shaderType::Type::Unlit)
      drawUnlitMaterialDescriptorEditor();
    else
    {
      String shaderTypeStr = shaderType::toString(m_currentShaderType);
      ImGui::Text("Unsupported material descriptor type: %s.", shaderTypeStr.c_str());
    }

    if (ImGui::Button("Save"))
      saveToFile();
  }

  void MaterialDescriptorEditorWindow::drawShaderTypeSelector()
  {
    if (ImGui::Combo("Shader Type", &m_selectedShaderTypeIndex, SHADER_TYPES, IM_ARRAYSIZE(SHADER_TYPES)))
      m_currentShaderType = shaderType::fromString(SHADER_TYPES[m_selectedShaderTypeIndex]);
  }

  void MaterialDescriptorEditorWindow::drawUnlitMaterialDescriptorEditor()
  {
    imguiUtilities::DrawColorEdit3("Color", m_unlitColor);

    if (!m_projectFileSelectorView)
      return;

    // file selection for main image
  }

  void MaterialDescriptorEditorWindow::updateShaderTypeCombo()
  {
    if (m_currentShaderType == shaderType::Type::Unlit)
      m_selectedShaderTypeIndex = 0;
    else
      m_selectedShaderTypeIndex = -1;
  }

  void MaterialDescriptorEditorWindow::saveToFile()
  {
    SharedPtr<MaterialDescriptor> materialDesc = createMaterialFromCurrentSettings();

    if (!materialDesc)
    {
      LogService::Error("Failed to create material descriptor from current settings.");
      return;
    }

    Optional<String> serializedContent =
      JsonSerializer::Serialize<MaterialDescriptor>(materialDesc.get());

    if (!serializedContent.has_value())
    {
      LogService::Error("Failed to serialize material descriptor to JSON.");
      return;
    }

    bool saveResult = fileUtilities::SaveStringToFile(
      m_materialDescriptor.getFilePath(),
      serializedContent.value()
    );

    if (!saveResult)
    {
      LogService::Error(
        String::Format(
          "Failed to save material descriptor to file: %s",
          m_materialDescriptor.getFilePath().string().c_str()
        )
      );
    }
  }

  SharedPtr<MaterialDescriptor> 
    MaterialDescriptorEditorWindow::createMaterialFromCurrentSettings()
  {
    if (m_currentShaderType == shaderType::Type::Unlit)
    {
      SharedPtr<UnlitMaterialDescriptor> unlitDesc =
        MakeShared<UnlitMaterialDescriptor>();

      unlitDesc->setColor(m_unlitColor);
      unlitDesc->setMainImageKey(m_unlitMainImageKey);

      return unlitDesc;
    }
    else
    {
      String shaderTypeStr = shaderType::toString(m_currentShaderType);
      LogService::Error(
        String::Format(
          "Couldn't create material descriptor. Unsupported shader type '%s'.",
          shaderTypeStr.c_str()
        )
      );

      return nullptr;
    }
  }
}
