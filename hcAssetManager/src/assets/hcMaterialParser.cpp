#include "hc/assets/hcMaterialParser.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

namespace hc
{
  namespace materialParser
  {
    SharedPtr<AMaterialDescriptor> ParseMaterialDescriptorFromAssimpMaterial(
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

    shadingType::Type DetermineShadingType(const aiMaterial* material)
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

    SharedPtr<AMaterialDescriptor> CreateDefaultMaterial()
    {
      return MakeShared<UnlitMaterialDescriptor>();
    }

    SharedPtr<AMaterialDescriptor> ParseUnlitMaterialDescriptor(
      const Path& fileDirectory,
      const aiMaterial* material
    )
    {
      SharedPtr<UnlitMaterialDescriptor> descriptor = 
        MakeShared<UnlitMaterialDescriptor>();

      aiColor3D color(1.0f, 1.0f, 1.0f);
      if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
      {
        descriptor->setColor(Color(color.r, color.g, color.b, 1.0f));
      }

      aiString texturePath;
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
      {
        Path textureRelativePath(texturePath.C_Str());
        descriptor->setMainImagePath(fileDirectory / textureRelativePath);
      }

      return descriptor;
    }
  }
}
