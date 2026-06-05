#include "hc/assets/model/hcAssimpMaterialDescriptorParser.h"
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
    shadingType::Type type = GetShadingTypeFromMaterial(material);

    switch (type)
    {
    case shadingType::Unlit:
      return ParseUnlitMaterialDescriptor(fileDirectory, material);
    case shadingType::BlinnPhong:
      return ParseBlinnPhongMaterialDescriptor(fileDirectory, material);
    default:
      return ParseUnlitMaterialDescriptor(fileDirectory, material);
    }
  }

  shadingType::Type AssimpMaterialDescriptorParser::GetShadingTypeFromMaterial(const aiMaterial* material)
  {
    if (!material)
      return shadingType::Unknown;

    int shadingModel = 0;
    if (material->Get(AI_MATKEY_SHADING_MODEL, shadingModel) != aiReturn_SUCCESS)
      return shadingType::Unknown;

    switch (shadingModel)
    {
    case aiShadingMode_Phong:
      return shadingType::BlinnPhong;
    case aiShadingMode_Blinn:
      return shadingType::BlinnPhong;
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
    return MakeShared<UnlitMaterialDescriptor>(
      "",
      GetVertexColorDiffuseFromMaterial(material),
      GetTexturePathFromMaterial(fileDirectory, material, aiTextureType_DIFFUSE)
    );
  }

  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::ParseBlinnPhongMaterialDescriptor(
    const Path& fileDirectory,
    const aiMaterial* material
  )
  {
    return MakeShared<BlinnPhongMaterialDescriptor>(
      "",
      GetVertexColorDiffuseFromMaterial(material),
      GetShininessFromMaterial(material),
      GetTexturePathFromMaterial(fileDirectory, material, aiTextureType_DIFFUSE),
      GetTexturePathFromMaterial(fileDirectory, material, aiTextureType_NORMALS),
      GetTexturePathFromMaterial(fileDirectory, material, aiTextureType_SPECULAR)
    );
  }

  Color AssimpMaterialDescriptorParser::GetVertexColorDiffuseFromMaterial(
    const aiMaterial* material
  )
  {
    aiColor3D color(1.0f, 1.0f, 1.0f);
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
      return Color(color.r, color.g, color.b, 1.0f);
    return Color::White();
  }

  Path AssimpMaterialDescriptorParser::GetTexturePathFromMaterial(
    const Path& fileDirectory,
    const aiMaterial* material,
    aiTextureType textureType
  )
  {
    aiString texturePath;
    if (material->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
    {
      Path textureRelativePath(texturePath.C_Str());
      return fileDirectory / textureRelativePath;
    }
    return Path();
  }

  float AssimpMaterialDescriptorParser::GetShininessFromMaterial(
    const aiMaterial* material
  )
  {
    float shininess = 0.0f;
    bool hasShininess = material->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS;
    if (!hasShininess)
      return 16.0f;

    // Blender FBX exports shininess as (1 - roughness)^2 * 100.
    // sqrt(x / 100) undoes the squaring, recovering the linear [0, 1]
    // shininess factor, which is then scaled to the Phong exponent range.
    shininess = Math::Sqrt(shininess / 100.0f) * 256.0f;

    return Math::Clamp(shininess, 1.0f, 256.0f);
  }
}
