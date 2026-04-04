#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"
#include "hc/editor/hcAssetFileReference.h"
#include "hc/editor/materialDescriptorEditor/hcIMaterialDescriptorEditor.h"

namespace hc::editor
{
  class ProjectFileDialogView;

  /**
   * @brief Window for editing material descriptors.
   *
   * Provides UI and logic for loading, editing, and saving material descriptor
   * assets. Supports multiple shader types and delegates editing to the
   * appropriate editor.
   */
  class MaterialDescriptorEditorWindow : public AWindowView
  {
  public:
    MaterialDescriptorEditorWindow(
      IAssetManager& assetManager,
      ProjectFileDialogView& projectFileSelector
    );
    virtual ~MaterialDescriptorEditorWindow();

    void destroy() override;

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
    IAssetManager& m_assetManager;
    UnorderedMap<shadingType::Type, UniquePtr<IMaterialDescriptorEditor>> m_editors;
    UniquePtr<IMaterialDescriptorEditor> m_nullEditor;
    shadingType::Type m_currentShaderType = shadingType::Type::Unknown;
    IMaterialDescriptorEditor* m_activeEditor = nullptr;
    AssetFileReference<AMaterialDescriptor> m_assetReference;
    Int32 m_selectedShaderTypeIndex = 0;

    void onDraw() override;
    void registerEditors(ProjectFileDialogView& projectFileSelector);
    void initializeEditors();
    IMaterialDescriptorEditor* getEditor(shadingType::Type type);
    void drawShaderTypeSelector();
    void updateShaderTypeCombo();
    void saveToFile();
  };
}
