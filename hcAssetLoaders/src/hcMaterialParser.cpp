#include "hc/hcMaterialParser.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

namespace hc
{
  namespace materialParser
  {
    SharedPtr<MaterialDescriptor> ParseMaterialDescriptorFromAssimpMaterial(
      const aiMaterial* material
    )
    {
      shadingType::Type type = DetermineShadingType(material);

      switch (type)
      {
      case shadingType::Unlit:
        return ParseUnlitMaterialDescriptor(material);
      default:
        return CreateDefaultMaterial();
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

    SharedPtr<MaterialDescriptor> CreateDefaultMaterial()
    {
      return MakeShared<UnlitMaterialDescriptor>();
    }

    SharedPtr<MaterialDescriptor> ParseUnlitMaterialDescriptor(
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
        descriptor->setMainImagePath(Path(texturePath.C_Str()));
      }

      return descriptor;
    }
  }
}
