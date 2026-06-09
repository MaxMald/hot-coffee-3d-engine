#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"

#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/graphics/cubeMap/hcCubeMapFactory.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"

namespace hc::serialization
{
  void SkyboxSerializer::Serialize(const Skybox& skybox, BinaryWriter& writer)
  {
    if (!skybox.hasCubeMap())
    {
      writer.writeBool(false);
      return;
    }

    const ICubeMap& cubeMap = skybox.getCubeMap();
    writer.writePath(cubeMap.getCubeMapDescriptorSourcePath());
  }

  void SkyboxSerializer::Deserialize(
    Skybox& skybox,
    BinaryReader& reader,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  )
  {
    if (!reader.readBool())
    {
      skybox.destroy();
      return;
    }

    Path cubeMapDescriptorSourcePath = reader.readPath();
    if (cubeMapDescriptorSourcePath.empty())
    {
      skybox.destroy();
      return;
    }

    try
    {
      SharedPtr<ICubeMap> cubeMap = CubeMapFactory::CreateFromDescriptor(
        cubeMapDescriptorSourcePath, assetManager, graphicsManager
      );

      skybox.destroy();
      skybox.initialize(cubeMap);
    }
    catch (const Exception&)
    {
      skybox.destroy();
      throw;
    }
  }
}
