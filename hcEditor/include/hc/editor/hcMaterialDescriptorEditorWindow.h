#pragma once

#include <hc/hcIDependencyResolvable.h>
#include <hc/hcMaterialDescriptor.h>
#include <hc/hcColor.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcAssetFileReference.h"

namespace hc::editor
{
  class MaterialDescriptorEditorWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    MaterialDescriptorEditorWindow();
    virtual ~MaterialDescriptorEditorWindow();

    void open(const Path& materialDescriptorPath);
    void resolveDependencies(DependencyContainer& container) override;
    void clear();

  private:
    AssetFileReference<MaterialDescriptor> m_materialDescriptor;
    shaderType::Type m_currentShaderType = shaderType::Type::Unknown;
    Int32 m_selectedShaderTypeIndex = 0;

    // Unlit Properties
    Color m_unlitColor;
    String m_unlitMainImageKey;

    void onDraw() override;
    void drawShaderTypeSelector();
    void drawUnlitMaterialDescriptorEditor();
    void updateShaderTypeCombo();
    void saveToFile();
    SharedPtr<MaterialDescriptor> createMaterialFromCurrentSettings();
  };
}
