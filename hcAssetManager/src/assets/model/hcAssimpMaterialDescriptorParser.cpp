#include "hc/assets/model/hcAssimpMaterialDescriptorParser.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

namespace hc
{
  static constexpr const char* SUFFIX_TRANSPARENT = "_Transparent";
  static constexpr const char* SUFFIX_ALPHA_CUTOUT = "_AlphaCutout";
  static constexpr const char* SUFFIX_DOUBLE_SIDED = "_DoubleSided";


  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::Parse(
    const Path& fileDirectory,
    const aiMaterial* material
  )
  {
    shadingType::Type type = GetShadingTypeFromMaterial(material);
    String name = GetMaterialNameFromMaterial(material);
    SharedPtr<AMaterialDescriptor> matDescriptor;

    switch (type)
    {
    case shadingType::Unlit:
      matDescriptor = ParseUnlitMaterialDescriptor(fileDirectory, name, material);
      break;
    case shadingType::BlinnPhong:
      matDescriptor = ParseBlinnPhongMaterialDescriptor(fileDirectory, name, material);
      break;
    default:
      matDescriptor = ParseUnlitMaterialDescriptor(fileDirectory, name, material);
      break;
    }

    ParseCommonMaterialPropertiesFromMaterial(material, matDescriptor);
    return matDescriptor;
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

  String AssimpMaterialDescriptorParser::GetMaterialNameFromMaterial(const aiMaterial* material)
  {
    aiString name;
    if (material->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS)
      return String(name.C_Str());
    return String();
  }

  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::ParseUnlitMaterialDescriptor(
    const Path& fileDirectory,
    const String& name,
    const aiMaterial* material
  )
  {
    return MakeShared<UnlitMaterialDescriptor>(
      "",
      name,
      GetVertexColorDiffuseFromMaterial(material),
      GetTexturePathFromMaterial(fileDirectory, material, aiTextureType_DIFFUSE)
    );
  }

  SharedPtr<AMaterialDescriptor> AssimpMaterialDescriptorParser::ParseBlinnPhongMaterialDescriptor(
    const Path& fileDirectory,
    const String& name,
    const aiMaterial* material
  )
  { 
    return MakeShared<BlinnPhongMaterialDescriptor>(
      "",
      name,
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

  void AssimpMaterialDescriptorParser::ParseCommonMaterialPropertiesFromMaterial(
    const aiMaterial* material,
    SharedPtr<AMaterialDescriptor>& materialDescriptor
  )
  {
    materialDescriptor->setDoubleSided(GetDoubleSidedFromMaterial(material));

    materialRenderMode::Type renderMode = GetRenderModeFromMaterial(material);
    materialDescriptor->setRenderMode(renderMode);

    if (renderMode == materialRenderMode::Type::AlphaCutout)
      materialDescriptor->setAlphaCutoutThreshold(GetAlphaCutoutThresholdFromMaterial(material));
  }

  bool AssimpMaterialDescriptorParser::GetDoubleSidedFromMaterial(
    const aiMaterial* material
  )
  {
    aiString name;
    if (material->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS)
    {
      String nameStr(name.C_Str());
      if (nameStr.find(SUFFIX_DOUBLE_SIDED) != String::npos)
        return true;
    }

    Int32 doubleSided = 0;
    material->Get(AI_MATKEY_TWOSIDED, doubleSided);
    return static_cast<bool>(doubleSided);
  }

  materialRenderMode::Type AssimpMaterialDescriptorParser::GetRenderModeFromMaterial(
    const aiMaterial* material
  )
  {
    aiString name;
    if (material->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS)
    {
      std::string nameStr(name.C_Str());
      if (nameStr.find(SUFFIX_ALPHA_CUTOUT) != std::string::npos)
        return materialRenderMode::Type::AlphaCutout;
      if (nameStr.find(SUFFIX_TRANSPARENT) != std::string::npos)
        return materialRenderMode::Type::Transparent;
    }

    // Fallback: If opacity was specified and is less than 1.0, treat the material as
    // transparent

    float opacity = 1.0f;
    material->Get(AI_MATKEY_OPACITY, opacity);
    if (opacity < 1.0f)
      return materialRenderMode::Type::Transparent;

    return materialRenderMode::Type::Opaque;
  }

  float AssimpMaterialDescriptorParser::GetAlphaCutoutThresholdFromMaterial(
    const aiMaterial* material
  )
  {
    aiString name;
    if (material->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS)
    {
      String nameStr(name.C_Str());
      if (nameStr.find(SUFFIX_ALPHA_CUTOUT) != String::npos)
      {
        // Extract the alpha cutout threshold from the material name suffix
        SizeT suffixPos = nameStr.find(SUFFIX_ALPHA_CUTOUT);
        String thresholdStr = nameStr.substr(suffixPos + strlen(SUFFIX_ALPHA_CUTOUT));
        try
        {
          float threshold = std::stof(thresholdStr);
          return Math::Clamp(threshold * 0.01f, 0.0f, 1.0f);
        }
        catch (const Exception&)
        {
          return 0.5f;
        }
      }
    }

    return 0.5f;
  }
}
