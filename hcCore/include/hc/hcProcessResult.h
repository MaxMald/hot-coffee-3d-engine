#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Represents the result of a processing operation.
   *
   * This struct is used to indicate whether an operation was successful and to provide
   * an optional message describing the result. It is commonly used for returning
   * status and error information from functions and processes.
   */
  struct ProcessResult
  {
    /**
     * @brief Indicates whether the operation was successful.
     */
    bool success;

    /**
     * @brief Message describing the result or error details.
     */
    String message;

    /**
     * @brief Constructs a ProcessResult.
     * 
     * @param success True if the operation succeeded, false otherwise.
     * @param message Optional message describing the result.
     */
    ProcessResult(bool success = true, const String& message = "") :
      success(success),
      message(message)
    {
    }
  };
}
