#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  /**
   * @brief Interface for a data block resource in the rendering engine.
   *
   * IDataBlock represents a GPU resource that holds structured data, such as camera
   * parameters, light information, or material properties. It provides methods for
   * initializing, uploading, and binding the data block to the GPU.
   *
   * IDataBlocks have fixed sizes and are typically used to store data that is frequently
   * accessed by shaders.
   */
  class HC_CORE_EXPORT IDataBlock : public IGraphicResource
  {
  public:
    virtual ~IDataBlock() override;
    
    /**
     * @brief Initializes the data block with a initial data buffer and a size which is
     * fixed and cannot be changed later.
     *
     * @param data Pointer to the initial data.
     * @param dataSize Size of this data in bytes.
     */
    virtual void initialize(const void* data, SizeT dataSize) = 0;

    /**
     * @brief Uploads data to the data block.
     *
     * @param data Pointer to the data to be uploaded.
     */
    virtual void upload(const void* data) = 0;

    /**
     * @brief Binds the data block to the specified binding index for use in shaders.
     *
     * @param bindingIndex The index to bind the data block to.
     */
    virtual void bind(UInt32 bindingIndex) = 0;

  protected:
    IDataBlock();
  };
}
