#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"
#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"

namespace hc::serialization
{
  void SkyboxSerializer::Serialize(const Skybox& skybox, BinaryWriter& writer)
  {
    const ICubeMap& cubeMap = skybox.getCubeMap();
    if (!skybox.isInitialized())
    {
      writer.writeBool(false);
      return;
    }

    writer.writeUInt32(cubeMap.getFaceWidth());
    writer.writeUInt32(cubeMap.getFaceHeight());
    writer.writeUInt8(cubeMap.getChannels());
    writer.writePath(skybox.getRightImagePath());
    writer.writePath(skybox.getLeftImagePath());
    writer.writePath(skybox.getTopImagePath());
    writer.writePath(skybox.getBottomImagePath());
    writer.writePath(skybox.getBackImagePath());
    writer.writePath(skybox.getFrontImagePath());
  }

  void SkyboxSerializer::Deserialize(
    Skybox& skybox,
    BinaryReader& reader,
    IAssetManager& assetManager
  )
  {
    if (!reader.readBool())
    {
      skybox.destroy();
      return;
    }

    UInt32 width = reader.readUInt32();
    UInt32 height = reader.readUInt32();
    UInt8 channels = reader.readUInt8();
    Path rightImagePath = reader.readPath();
    Path leftImagePath = reader.readPath();
    Path topImagePath = reader.readPath();
    Path bottomImagePath = reader.readPath();
    Path backImagePath = reader.readPath();
    Path frontImagePath = reader.readPath();

    IImageAssetManager& imageAssetManager = assetManager.getImageAssetManager();

    SharedPtr<Image> rightImage = imageAssetManager.load(rightImagePath);
    SharedPtr<Image> leftImage = imageAssetManager.load(leftImagePath);
    SharedPtr<Image> topImage = imageAssetManager.load(topImagePath);
    SharedPtr<Image> bottomImage = imageAssetManager.load(bottomImagePath);
    SharedPtr<Image> backImage = imageAssetManager.load(backImagePath);
    SharedPtr<Image> frontImage = imageAssetManager.load(frontImagePath);

    SharedPtr<Image> defaultImage;
    if (!rightImage || !leftImage || !topImage || !bottomImage || !backImage || !frontImage)
    {
      BufferByte defaultImageData(width * height * channels);
      for (SizeT i = 0; i < defaultImageData.size(); ++i)
        defaultImageData[i] = 255;

      defaultImage = MakeShared<Image>(
        "",
        width, height, channels,
        std::move(defaultImageData)
      );
    }

    if (!rightImage)
    {
      rightImage = defaultImage;
      LogService::Error("Failed to load right image for skybox: " + rightImagePath.string() +
        ". Using default white image instead."
      );
    }

    if (!leftImage)
    {
      leftImage = defaultImage;
      LogService::Error("Failed to load left image for skybox: " + leftImagePath.string() +
        ". Using default white image instead."
      );
    }

    if (!topImage)
    {
      topImage = defaultImage;
      LogService::Error("Failed to load top image for skybox: " + topImagePath.string() +
        ". Using default white image instead."
      );
    }

    if (!bottomImage)
    {
      bottomImage = defaultImage;
      LogService::Error("Failed to load bottom image for skybox: " + bottomImagePath.string() +
        ". Using default white image instead."
      );
    }

    if (!backImage)
    {
      backImage = defaultImage;
      LogService::Error("Failed to load back image for skybox: " + backImagePath.string() +
        ". Using default white image instead."
      );
    }

    if (!frontImage)
    {
      frontImage = defaultImage;
      LogService::Error("Failed to load front image for skybox: " + frontImagePath.string() +
        ". Using default white image instead."
      );
    }

    skybox.destroy();
    skybox.initialize(
      width, height, channels,
      *rightImage, *leftImage, *topImage,
      *bottomImage, *backImage, *frontImage
    );
  }
}
