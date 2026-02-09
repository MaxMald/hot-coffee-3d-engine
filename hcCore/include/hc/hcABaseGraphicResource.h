#pragma once

#include "hc/hcIGraphicResource.h"

namespace hc
{
  /**
   * @brief Abstract base class for graphic resources.
   *
   * Provides a common implementation for storing and retrieving a unique
   * resource identifier. Inherits from IGraphicResource and implements getId().
   */
  class HC_CORE_EXPORT ABaseGraphicResource : public IGraphicResource 
  {
  public:
    virtual ~ABaseGraphicResource() = default;

    /**
     * @brief Returns the unique identifier of the graphic resource.
     *
     * @return Reference to the resource's Id.
     */
    const Id& getId() const final;

  protected:
    ABaseGraphicResource();
    ABaseGraphicResource(const Id& id);

  private:
    /**
     * @brief Unique identifier for the graphic resource.
     */
    Id m_id;
  };
}
