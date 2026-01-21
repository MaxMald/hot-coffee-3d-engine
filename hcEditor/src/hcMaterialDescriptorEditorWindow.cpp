#include "hc/editor/hcMaterialDescriptorEditorWindow.h"
#include <hc/hcMaterialDescriptor.h>
#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

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

  void MaterialDescriptorEditorWindow::resolveDependencies(
    DependencyContainer& container
  )
  {
  }

  void MaterialDescriptorEditorWindow::setMaterialDescriptor(
    const AssetFileReference<MaterialDescriptor>& materialDescriptor
  )
  {
    m_materialDescriptor = materialDescriptor;
    m_currentShaderType = materialDescriptor.getAsset()->getShaderType();
  }

  void MaterialDescriptorEditorWindow::onDraw()
  {
    if (!m_materialDescriptor.getAsset())
    {
      ImGui::Text("No material descriptor loaded.");
      return;
    }

    // TODO Draw shader type editor

    switch (m_currentShaderType)
    {
    case shaderType::Type::Unlit:
      drawUnlitMaterialDescriptorEditor();
      break;

    default:
    {
      String shaderTypeStr = shaderType::toString(m_currentShaderType);
      ImGui::Text("Unsupported material descriptor type.");
      break;
    } 
    }
  }

  void MaterialDescriptorEditorWindow::drawUnlitMaterialDescriptorEditor()
  {
    imguiUtilities::DrawColorEdit3("Color", m_unlitColor);
  }

  void MaterialDescriptorEditorWindow::saveToFile()
  {
  }
}
