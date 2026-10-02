#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"

#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/resource/cubeMap/hcICubeMap.h"
#include "hc/graphics/resource/cubeMap/hcCubeMapFactory.h"
#include "hc/graphics/generators/hcIEquirectangularToCubeMapGenerator.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"
#include "hc/serialization/hcFileFormats.h"

namespace hc::serialization
{
  static constexpr UInt32 SKYBOX_SERIALIZATION_VERSION = 1;

  void SkyboxSerializer::Serialize(
    const Skybox& skybox,
    io::BinaryWriter& writer,
    const IAssetManager& assetManager
  )
  {
    writer.startWritingObject(static_cast<UInt32>(0), SKYBOX_SERIALIZATION_VERSION);

    if (!skybox.hasCubeMap())
    {
      writer.writeBool(false);
      writer.finishWritingObject();
      return;
    }
    
    if (skybox.sourcePath.empty())
    {
      writer.writeBool(false);
      writer.finishWritingObject();
      return;
    }

    Path pathToSerialize = skybox.sourcePath;
    if (assetManager.hasRootPath())
      pathToSerialize = skybox.sourcePath.toRelative(assetManager.getRootPath());

    writer.writeBool(true);
    writer.writePath(pathToSerialize);
    writer.finishWritingObject();
  }

  void SkyboxSerializer::Deserialize(
    Skybox& skybox,
    io::BinaryReader& reader,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  )
  {
    skybox.destroy();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(SKYBOX_SERIALIZATION_VERSION))
    { 
      reader.finishReadingObject();
      return;
    }

    if (!reader.readBool())
    {
      reader.finishReadingObject();
      return;
    }

    Path sourcePath = reader.readPath();
    reader.finishReadingObject();

    if (sourcePath.empty())
      return;

    if (sourcePath.isRelative())
    {
      if (!assetManager.hasRootPath())
      {
        throw RuntimeErrorException(
          "Failed to deserialize skybox: source path is relative but asset manager has no root path set. Source path: " + sourcePath.toString()
        );
      }

      const Path& rootPath = assetManager.getRootPath();
      sourcePath = sourcePath.toAbsolute(rootPath);
    }

    IImageAssetManager& imageAssetManager = assetManager.getImageAssetManager();

    bool isCubeMapDescriptor = false;
    Path extension = sourcePath.extension().toLowercase();
    if (extension == serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION)
      isCubeMapDescriptor = true;
    else if (imageAssetManager.isSupportedImage(sourcePath))
      isCubeMapDescriptor = false;
    else
    {
      throw RuntimeErrorException(
        String::Format("Failed to deserialize skybox: source path '%s' is neither a supported image nor a cube map descriptor.",
          sourcePath.toString().c_str()
        )
      );
    }

    try
    {
      if (isCubeMapDescriptor)
      {
        SharedPtr<ICubeMap> cubeMap = CubeMapFactory::CreateFromDescriptor(
          sourcePath, assetManager, graphicsManager
        );
        skybox.initialize(cubeMap);
      }
      else
      {
        SharedPtr<ICubeMap> cubeMap = CubeMapFactory::CreateFromEquirectangularImage(
          sourcePath, 2048, assetManager, graphicsManager
        );
        skybox.initialize(cubeMap);
      }
    }
    catch (const Exception& ex)
    {
      skybox.destroy();
      throw RuntimeErrorException(
        "Failed to create cube map from descriptor at path: " + sourcePath.toString() +
        ". Error: " + String(ex.what())
      );
    }
  }
}
