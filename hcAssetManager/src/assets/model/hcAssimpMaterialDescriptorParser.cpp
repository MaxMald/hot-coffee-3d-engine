#include "hc/assets/hcAssimpMaterialDescriptorParser.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

namespace hc
{
  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::Parse(
    const Path& fileDirectory,
    const aiMaterial* material
  )
  {
    shadingType::Type type = DetermineShadingType(material);

    switch (type)
    {
    default:
      return ParseUnlitMaterialDescriptor(fileDirectory, material);
    }
  }

  shadingType::Type AssimpMaterialDescriptorParser::DetermineShadingType(const aiMaterial* material)
  {
    if (!material)
      return shadingType::Unknown;

    int shadingModel = 0;
    if (material->Get(AI_MATKEY_SHADING_MODEL, shadingModel) != aiReturn_SUCCESS)
      return shadingType::Unknown;

    switch (shadingModel)
    {
    case aiShadingMode_Blinn:
      return shadingType::Unlit;
    case aiShadingMode_NoShading:
      return shadingType::Unlit;
    default:
      return shadingType::Unknown;
    }
  }

  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::ParseUnlitMaterialDescriptor(
    const Path& fileDirectory,
    const aiMaterial* material
  )
  {
    aiColor3D color(1.0f, 1.0f, 1.0f);
    Color hcColor;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
    {
      hcColor = Color(color.r, color.g, color.b, 1.0f);
    }

    aiString texturePath;
    Path mainImagePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
    {
      Path textureRelativePath(texturePath.C_Str());
      mainImagePath = fileDirectory / textureRelativePath;
    }

    SharedPtr<UnlitMaterialDescriptor> descriptor =
      MakeShared<UnlitMaterialDescriptor>("", hcColor, mainImagePath);

    return descriptor;
  }
}
