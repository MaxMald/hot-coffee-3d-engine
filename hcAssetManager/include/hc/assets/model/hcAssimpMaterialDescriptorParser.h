#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

struct aiMaterial;
enum aiTextureType;

namespace hc
{
  /**
   * Utility for parsing Assimp materials into engine material descriptors.
   *
   * Converts Assimp aiMaterial structures into the appropriate engine
   * material descriptor types based on their shading models and
   * properties.
   */
  struct AssimpMaterialDescriptorParser
  {
  public:
    /**
     * Parses an Assimp material into an engine material descriptor.
     *
     * Analyzes the Assimp material to determine its shading type and
     * creates the corresponding material descriptor with all relevant
     * properties and texture paths.
     *
     * @param fileDirectory The directory containing the model file, used
     * for resolving relative texture paths
     * @param material The Assimp material to parse
     * 
     * @return A shared pointer to the created material descriptor
     */
    static SharedPtr<AMaterialDescriptor> Parse(
      const Path& fileDirectory,
      const aiMaterial* material
    );

  private:
    /**
     * Determines the shading type from an Assimp material.
     *
     * @param material The Assimp material to analyze
     * 
     * @return The corresponding engine shading type
     */
    static shadingType::Type GetShadingTypeFromMaterial(const aiMaterial* material);

    /**
     * Parses an unlit material descriptor from an Assimp material.
     *
     * @param fileDirectory The directory for resolving texture paths
     * @param material The Assimp material to parse
     * 
     * @return A shared pointer to the created unlit material descriptor
     */
    static SharedPtr<AMaterialDescriptor> ParseUnlitMaterialDescriptor(
      const Path& fileDirectory,
      const aiMaterial* material
    );

    static SharedPtr<AMaterialDescriptor> ParseBlinnPhongMaterialDescriptor(
      const Path& fileDirectory,
      const aiMaterial* material
    );

    static Color GetVertexColorDiffuseFromMaterial(const aiMaterial* material);
    static Path GetTexturePathFromMaterial(
      const Path& fileDirectory,
      const aiMaterial* material,
      aiTextureType textureType
    );
  };
}
