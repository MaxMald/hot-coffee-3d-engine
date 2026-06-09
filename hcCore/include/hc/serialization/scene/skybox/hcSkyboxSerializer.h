#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Skybox;
  class IAssetManager;
  class IGraphicsManager;

  namespace serialization
  {
    struct HC_CORE_EXPORT SkyboxSerializer
    {
    public:
      static void Serialize(
        const Skybox& skybox,
        BinaryWriter& writer,
        const IAssetManager& assetManager
      );
      static void Deserialize(
        Skybox& skybox,
        BinaryReader& reader,
        IAssetManager& assetManager,
        IGraphicsManager& graphicsManager
      );
    };
  }
}
