#include "hc/graphics/resource/shader/hcShaderIncluder.h"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace hc
{
  /**
   * @brief A structure to hold data related to shader includes.
   */
  struct IncludeData
  {
    String sourceName;
    String content;
  };

  /**
   * @brief Creates a shaderc_include_result for a successful include operation.
   *
   * @param sourcePath The path to the included source file (absolute path).
   * @param content The content of the included source file.
   *
   * @returns A pointer to a shaderc_include_result containing the include data.
   */
  static shaderc_include_result* CreateContentResult(
    const Path& sourcePath,
    const String& content
  )
  {
    IncludeData* includeData = new IncludeData();
    includeData->sourceName = sourcePath.string();
    includeData->content = content;

    shaderc_include_result* result = new shaderc_include_result();
    result->source_name = includeData->sourceName.c_str();
    result->source_name_length = includeData->sourceName.size();
    result->content = includeData->content.c_str();
    result->content_length = includeData->content.size();
    result->user_data = includeData;
    return result;
  }

  /**
   * @brief Creates a shaderc_include_result for an error during include operation.
   *
   * @param errorMessage The error message to be included in the result.
   *
   * @returns A pointer to a shaderc_include_result containing the error data.
   */
  static shaderc_include_result* CreateErrorResult(const String& errorMessage)
  {
    IncludeData* includeData = new IncludeData();
    includeData->sourceName = "";
    includeData->content = errorMessage;

    shaderc_include_result* result = new shaderc_include_result();
    result->source_name = includeData->sourceName.c_str();
    result->source_name_length = includeData->sourceName.size();
    result->content = includeData->content.c_str();
    result->content_length = includeData->content.size();
    result->user_data = includeData;
    return result;
  }

  shaderc_include_result* ShaderIncluder::GetInclude(
    const char* requestedSource,
    shaderc_include_type type,
    const char* requestingSource,
    size_t includeDepth
  )
  {
    if (shaderc_include_type::shaderc_include_type_standard == type)
    {
      return CreateErrorResult("Standard includes are not supported.");
    }

    Path requestingSourcePath = std::filesystem::path(requestingSource).parent_path();
    Path requestedSourcePath = requestingSourcePath / requestedSource;

    std::ifstream file(requestedSourcePath, std::ios::in | std::ios::binary);
    if (!file)
    {
      return CreateErrorResult(
        String::Format(
          "Failed to open the requested source file: %s",
          requestedSourcePath.string().c_str()
        )
      );
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

    return CreateContentResult(requestedSourcePath, contents.str());
  }

  void ShaderIncluder::ReleaseInclude(shaderc_include_result* data)
  {
    if (data)
    {
      IncludeData* includeData = static_cast<IncludeData*>(data->user_data);
      delete includeData;
      delete data;
    }
  }
}
