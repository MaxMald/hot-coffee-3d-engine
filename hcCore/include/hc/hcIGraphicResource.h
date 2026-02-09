#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Interface for graphic resource objects.
   *
   * Represents a generic graphic resource with a unique identifier. Provides
   * methods for accessing the resource ID and destroying the resource.
   */
  class IGraphicResource : public NonCopyable
  {
  public:
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~IGraphicResource() = default;

    /**
     * @brief Returns the unique identifier of the graphic resource.
     * 
     * @return Reference to the resource's Id.
     */
    virtual const Id& getId() const = 0;

    /**
     * @brief Destroys the graphic resource and releases associated resources.
     */
    virtual void destroy() = 0;

   protected:
     /**
      * @brief Protected default constructor to prevent direct instantiation.
      */
     IGraphicResource() = default;
  };
}
