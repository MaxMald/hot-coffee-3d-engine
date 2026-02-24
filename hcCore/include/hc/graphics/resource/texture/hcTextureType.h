#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace textureType
  {
    enum Type : UInt8
    {
      None = 0,
      Diffuse = 1,
      Specular = 2,
      Ambient = 3,
      Emissive = 4,
      Height = 5,
      Normals = 6,
      Shininess = 7,
      Opacity = 8,
      Displacement = 9,
      Lightmap = 10,
      Reflection = 11,

      // PBR
      BaseColor = 12,
      NormalCamera = 13,
      EmissionColor = 14,
      Metalness = 15,
      DiffuseRoughness = 16,
      AmbientOcclusion = 17,

      Sheen = 19,
      Clearcoat = 20,
      Transmission = 21,

      // Maya material declarations

      MayaBase = 22,
      MayaSpecular = 23,
      MayaSpecularColor = 24,
      MayaSpecularRoughness = 25,

      Anisotropy = 26,
      GltfMetallicRoughness = 27,

      Unknown = 255
    };
  }
}
