#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for editor services.
   *
   * All editor services must inherit from this interface. It provides a common
   * contract for destruction and ensures non-copyable semantics.
   */
  class IEditorService : public NonCopyable
  {
  public:
    /**
     * @brief Virtual destructor for safe cleanup of derived services.
     */
    virtual ~IEditorService() = default;

    /**
     * @brief Destroys the editor service and releases its resources.
     */
    virtual void destroy() = 0;

  protected:
    /**
     * @brief Protected default constructor to prevent direct instantiation.
     */
    IEditorService() = default;
  };
}
