#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"

#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/graphics/cubeMap/hcCubeMapFactory.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"

namespace hc::serialization
{
  void SkyboxSerializer::Serialize(
    const Skybox& skybox,
    io::BinaryWriter& writer,
    const IAssetManager& assetManager
  )
  {
    if (!skybox.hasCubeMap())
    {
      writer.writeBool(false);
      return;
    }

    writer.writeBool(true);
    const ICubeMap& cubeMap = skybox.getCubeMap();

    Path descriptorSourcePath = cubeMap.getCubeMapDescriptorSourcePath();
    Path pathToSerialize = descriptorSourcePath;

    if (assetManager.hasRootPath())
      pathToSerialize = descriptorSourcePath.toRelative(assetManager.getRootPath());

    writer.writeString(pathToSerialize);
  }

  void SkyboxSerializer::Deserialize(
    Skybox& skybox,
    io::BinaryReader& reader,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  )
  {
    if (!reader.readBool())
    {
      skybox.destroy();
      return;
    }

    Path sourcePath = reader.readPath();
    if (sourcePath.empty())
    {
      skybox.destroy();
      return;
    }

    if (sourcePath.isRelative())
    {
      if (!assetManager.hasRootPath())
      {
        skybox.destroy();
        throw RuntimeErrorException(
          "Cannot resolve relative asset path without a root path set in the asset manager."
        );
      }

      const Path& rootPath = assetManager.getRootPath();
      sourcePath = sourcePath.toAbsolute(rootPath);
    }

    try
    {
      SharedPtr<ICubeMap> cubeMap = CubeMapFactory::CreateFromDescriptor(
        sourcePath, assetManager, graphicsManager
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
