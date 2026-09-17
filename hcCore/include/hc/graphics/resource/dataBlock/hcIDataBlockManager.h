#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  /**
   * @brief Interface for managing data blocks in the rendering engine.
   */
  class HC_CORE_EXPORT IDataBlockManager
  {
  public:
    virtual ~IDataBlockManager();

    /**
     * @brief Initializes the data block manager and prepares it for use.
     */
    virtual void initialize() = 0;

    /**
     * @brief Uploads data to the data block of the specified type.
     *
     * For block data with matrix types, the matrix should be transposed if
     * shouldTransposeMatrices() returns true.
     *
     * @param dataBlockType The type of data block to upload data to.
     * @param data Pointer to the data to be uploaded.
     */
    virtual void upload(
      dataBlockType::Type dataBlockType,
      const void* data
    ) = 0;

    /**
     * @brief Determines whether matrices should be transposed before uploading.
     *
     * @return True if matrices should be transposed, false otherwise.
     */
    virtual bool shouldTransposeMatrices() const = 0;

    /**
     * @brief Binds the data block of the specified type to its default binding index.
     *
     * @param dataBlockType The type of data block to bind.
     */
    virtual void bind(dataBlockType::Type dataBlockType) = 0;

    /**
     * @brief Binds the data block of the specified type to the given binding index.
     *
     * @param dataBlockType The type of data block to bind.
     * @param bindingIndex The binding index to which the data block should be bound.
     */
    virtual void bind(dataBlockType::Type dataBlockType, UInt32 bindingIndex) = 0;

    /**
     * @brief Destroys the data block manager and releases all associated resources.
     */
    virtual void destroy() = 0;

  protected:
    IDataBlockManager();
  };
}
