#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/assets/hcPrimitiveShapeType.h"

namespace hc
{
  /**
   * Utilities for managing file paths of primitive model assets.
   * 
   * Provides constants and helper functions for constructing paths to
   * primitive shape models in the asset system.
   */
  struct PrimitiveModelPathUtilities
  {
    /**
     * Base directory path for primitive model assets.
     */
    static constexpr const char* PrimitivesBasePath = "_HC_PRIMITIVES";

    /**
     * Checks if a given path corresponds to a primitive model.
     * 
     * Determines whether the provided path points to a built-in
     * primitive shape asset based on the path structure.
     * 
     * @param path The file path to check.
     * 
     * @return True if the path is a primitive model path, false
     * otherwise.
     */
    static bool IsPrimitiveModelPath(const Path& path);

    /**
     * Extracts the primitive shape type from a primitive model path.
     * 
     * Parses the provided path and returns the corresponding primitive
     * shape type. The path must be a valid primitive model path.
     * 
     * @param path The primitive model path to parse.
     * 
     * @return The primitive shape type identified from the path.
     */
    static primitiveShapeType::Type GetPrimitiveShapeTypeFromPath(const Path& path);

    /**
     * Constructs the file path for a given primitive shape type.
     * 
     * Generates the full asset path for the specified primitive shape
     * type using the base primitives directory.
     * 
     * @param primitiveType The type of primitive shape.
     * 
     * @return The constructed file path for the primitive model.
     */
    static Path GetPrimitiveModelPath(primitiveShapeType::Type primitiveType);
  };
}
