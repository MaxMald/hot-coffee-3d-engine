#pragma once

#include <hc/graphics/resource/dataBlock/hcIDataBlock.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlDataBlock : public IDataBlock
  {
  public:
    OpenGlDataBlock();
    virtual ~OpenGlDataBlock() override;

    /**
     * @copydoc IGraphicResource::getId
     */
    const Id& getId() const override;

    /**
     * @copydoc IGraphicResource::isValid
     */
    bool isValid() const override;

    /**
     * @copydoc IGraphicResource::destroy
     */
    void destroy() override;

    /**
     * @copydoc IDataBlock::initialize
     */
    void initialize(const void* data, SizeT dataSize) override;

    /**
     * @copydoc IDataBlock::upload
     */
    void upload(const void* data) override;

    /**
     * @copydoc IDataBlock::bind
     */
    void bind(UInt32 bindingIndex) override;

  private:
    Id m_id;
    UInt32 m_ubo;
    SizeT m_dataSize;
    bool m_isInitialized;

    void assertIsValid() const;
  };
}
