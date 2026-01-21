#pragma once

#include <hc/hcIDependencyResolvable.h>
#include <hc/hcUnlitMaterialDescriptor.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcAssetFileReference.h"

namespace hc
{
  class UnlitMaterialDescriptor;
}

namespace hc::editor
{
  class MaterialDescriptorEditorWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    MaterialDescriptorEditorWindow();
    virtual ~MaterialDescriptorEditorWindow();

    void resolveDependencies(DependencyContainer& container) override;
    void setMaterialDescriptor(const AssetFileReference<MaterialDescriptor>& materialDescriptor);
    void clear();

  private:
    AssetFileReference<MaterialDescriptor> m_materialDescriptor;
    shaderType::Type m_currentShaderType = shaderType::Type::Unknown;

    // Unlit Properties
    Color m_unlitColor;
    String m_unlitMainImageKey;

    void onDraw() override;
    void drawUnlitMaterialDescriptorEditor();
    void saveToFile();
  };
}
