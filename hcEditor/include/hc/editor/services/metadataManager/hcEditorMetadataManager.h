#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/metadataManager/hcEditorModelMetadataManager.h"

namespace hc::editor
{
  class EditorMetadataManager : public IEditorService
  {
  public:
    EditorMetadataManager(IAssetManager& assetManager);
    virtual ~EditorMetadataManager() override;

    void prepare() override;
    void destroy() override;

    inline EditorModelMetadataManager& getModelMetadataManager()
    {
      return m_modelMetadataManager;
    }

  private:
    IAssetManager& m_assetManager;
    EditorModelMetadataManager m_modelMetadataManager;
  };
}
