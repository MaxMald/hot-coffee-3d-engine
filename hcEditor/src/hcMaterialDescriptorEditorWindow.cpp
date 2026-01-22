#include "hc/editor/hcMaterialDescriptorEditorWindow.h"

#include <hc/hcMaterialDescriptor.h>
#include <hc/hcLogService.h>
#include <hc/hcDependencyContainer.h>
#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include "hc/editor/hcImguiUtilities.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelectorView.h"
#include "hc/editor/hcUnlitMaterialDescriptorEditor.h"
#include "hc/editor/hcNullMaterialDescriptorEditor.h"
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
    m_assetReference(),
    m_currentShaderType(shaderType::Type::Unknown)
  {
    registerEditors();
  }

  MaterialDescriptorEditorWindow::~MaterialDescriptorEditorWindow()
  {
  }

  void MaterialDescriptorEditorWindow::open(const Path& materialDescriptorPath)
  {
    clear();

    try
    {
      SharedPtr<MaterialDescriptor> materialDescriptor = HotCoffeeEngine::Instance()
        .getAssetManager()
        .loadDirect<MaterialDescriptor>(materialDescriptorPath);

      if (!materialDescriptor)
        return;

      m_assetReference.setFilePath(materialDescriptorPath);
      m_assetReference.setAsset(SharedPtr<MaterialDescriptor>(materialDescriptor));
      m_currentShaderType = materialDescriptor->getShaderType();

      updateShaderTypeCombo();

      m_activeEditor = getEditor(m_currentShaderType);
      m_activeEditor->copyValuesFrom(m_assetReference.getAsset());

      m_isOpen = true;
    }
    catch (const std::exception& ex)
    {
      LogService::Error(
        String::Format(
          "Failed to open material descriptor file '%s': %s",
          materialDescriptorPath.string().c_str(),
          ex.what()
        )
      );
    }
  }

  void MaterialDescriptorEditorWindow::resolveDependencies(
    DependencyContainer& container
  )
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_projectFileSelectorView = container.resolve<ProjectFileSelectorView>();
    initializeEditors(m_projectFileSelectorView);
  }

  void MaterialDescriptorEditorWindow::clear()
  {
    m_assetReference = AssetFileReference<MaterialDescriptor>();
    m_currentShaderType = shaderType::Type::Unknown;
    m_activeEditor = nullptr;

    for (auto& [type, editor] : m_editors)
      editor->clear();
  }

  void MaterialDescriptorEditorWindow::registerEditors()
  {
    m_editors[shaderType::Type::Unlit] = MakeUnique<UnlitMaterialDescriptorEditor>();
    m_nullEditor = MakeUnique<NullMaterialDescriptorEditor>();
  }

  void MaterialDescriptorEditorWindow::initializeEditors(
    SharedPtr<ProjectFileSelectorView> projectFileSelector
  )
  {
    for (auto& [type, editor] : m_editors)
      editor->init(projectFileSelector);
  }

  IMaterialDescriptorEditor* MaterialDescriptorEditorWindow::getEditor(
    shaderType::Type type
  )
  {
    auto it = m_editors.find(type);
    if (it != m_editors.end())
      return it->second.get();

    return m_nullEditor.get();
  }

  void MaterialDescriptorEditorWindow::onDraw()
  {
    drawShaderTypeSelector();
    ImGui::SameLine();
    if (ImGui::Button("Save Changes"))
      saveToFile();
    ImGui::Separator();
    ImGui::Text("Material Properties:");

    if (m_activeEditor)
      m_activeEditor->draw();
  }

  void MaterialDescriptorEditorWindow::drawShaderTypeSelector()
  {
    ImGui::SetNextItemWidth(120.0f);
    if (ImGui::Combo("Shader Type", &m_selectedShaderTypeIndex, SHADER_TYPES, IM_ARRAYSIZE(SHADER_TYPES)))
      m_currentShaderType = shaderType::fromString(SHADER_TYPES[m_selectedShaderTypeIndex]);
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
    if (!m_activeEditor)
      return;

    m_activeEditor->save(m_assetReference.getFilePath());
  }
}
