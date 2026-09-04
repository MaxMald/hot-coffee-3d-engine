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
    bool m_isInitialized = false;
    UnorderedMap<dataBlockType::Type, UniquePtr<OpenGlDataBlock>> m_dataBlocks;

    OpenGlDataBlock* getData(dataBlockType::Type dataBlockType);
  };
}
