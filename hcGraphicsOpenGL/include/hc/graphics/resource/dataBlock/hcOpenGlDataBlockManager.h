#pragma once

#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcOpenGlDataBlock.h"

namespace hc
{
  class OpenGlDataBlockManager : public IDataBlockManager
  {
  public:
    void initialize() override;
    void upload(dataBlockType::Type dataBlockType, const void* data) override;
    bool shouldTransposeMatrices() const override;
    void bind(dataBlockType::Type dataBlockType) override;
    void bind(dataBlockType::Type dataBlockType, UInt32 bindingIndex) override;
    void destroy() override;

  private:
    UnorderedMap<dataBlockType::Type, UniquePtr<OpenGlDataBlock>> m_dataBlocks;
    bool m_isInitialized = false;

    OpenGlDataBlock* getDataBlock(dataBlockType::Type dataBlockType);
    void createDataBlock(dataBlockType::Type dataBlockType, const void* initialData);
  };
}
