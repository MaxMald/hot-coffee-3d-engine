#pragma once

#include <shaderc/shaderc.hpp>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief A custom includer for handling #include directives in shader source code.
   */
  class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
  {
  public:

    /**
     * @brief Resolves an #include request and returns the corresponding include result.
     *
     * @param requestedSource The name of the source being requested.
     * @param type The type of inclusion request (standard or relative).
     * @param requestingSource The name of the source containing the #include request.
     * @param includeDepth The depth of the include request (for nested includes).
     *
     * @returns A pointer to a shaderc_include_result containing the include data.
     */
    shaderc_include_result* GetInclude(
      const char* requestedSource,
      shaderc_include_type type,
      const char* requestingSource,
      size_t includeDepth
    ) override;

    /**
     * @brief Releases the resources held by the include result.
     *
     * @param data A pointer to the shaderc_include_result to be released.
     */
    void ReleaseInclude(shaderc_include_result* data) override;
  };
}
