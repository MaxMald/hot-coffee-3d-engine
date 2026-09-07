#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"

#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/graphics/cubeMap/hcCubeMapFactory.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"
#include "hc/assets/hcAssetPath.h"

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
    String pathToSerialize = descriptorSourcePath.generic_string();

    if (assetManager.hasRootPath())
    {
      const Path& rootPath = assetManager.getRootPath();
      pathToSerialize = AssetPath::ToRelative(descriptorSourcePath, rootPath);
    }

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

    String sourcePathStr = reader.readString();
    if (sourcePathStr.empty())
    {
      skybox.destroy();
      return;
    }

    Path sourcePath(sourcePathStr.c_str());
    if (AssetPath::IsRelative(sourcePath))
    {
      if (!assetManager.hasRootPath())
      {
        skybox.destroy();
        throw RuntimeErrorException(
          "Cannot resolve relative asset path without a root path set in the asset manager."
        );
      }

      const Path& rootPath = assetManager.getRootPath();
      sourcePath = AssetPath::ToAbsolute(sourcePathStr, rootPath);
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
