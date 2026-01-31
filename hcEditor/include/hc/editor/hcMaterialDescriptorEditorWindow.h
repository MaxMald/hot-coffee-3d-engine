#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcAssetFileReference.h"
#include "hc/editor/hcIMaterialDescriptorEditor.h"

namespace hc::editor
{
  class ProjectFileSelectorView;

  /**
   * @brief Window for editing material descriptors.
   *
   * Provides UI and logic for loading, editing, and saving material descriptor
   * assets. Supports multiple shader types and delegates editing to the
   * appropriate editor.
   */
  class MaterialDescriptorEditorWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    MaterialDescriptorEditorWindow();
    virtual ~MaterialDescriptorEditorWindow();

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @brief Opens a material descriptor for editing.
     * 
     * @param materialDescriptorPath Path to the material descriptor file.
     */
    void open(const Path& materialDescriptorPath);

    /**
     * @brief Clears the editor state and resets all values.
     */
    void clear();

  private:
    SharedPtr<ProjectFileSelectorView> m_projectFileSelectorView;
    UnorderedMap<shaderType::Type, UniquePtr<IMaterialDescriptorEditor>> m_editors;
    UniquePtr<IMaterialDescriptorEditor> m_nullEditor;
    shaderType::Type m_currentShaderType = shaderType::Type::Unknown;
    IMaterialDescriptorEditor* m_activeEditor = nullptr;
    AssetFileReference<MaterialDescriptor> m_assetReference;
    Int32 m_selectedShaderTypeIndex = 0;

    void onDraw() override;
    void registerEditors();
    void initializeEditors(SharedPtr<ProjectFileSelectorView> projectFileSelector);
    IMaterialDescriptorEditor* getEditor(shaderType::Type type);
    void drawShaderTypeSelector();
    void updateShaderTypeCombo();
    void saveToFile();
  };
}
