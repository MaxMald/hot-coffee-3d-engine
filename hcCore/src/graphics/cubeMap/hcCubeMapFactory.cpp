#include "hc/graphics/cubeMap/hcCubeMapFactory.h"

#include "hc/utilities/hcTextureFormatType.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"
#include "hc/assets/cubeMapDescriptor/hcICubeMapDescriptorAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"
#include "hc/graphics/hcIGraphicsManager.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"

namespace hc
{
  SharedPtr<ICubeMap> CubeMapFactory::CreateFromDescriptor(
    const Path& cubeMapDescriptorSourcePath,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  )
  {
    SharedPtr<CubeMapDescriptor> descriptor = assetManager
      .getCubeMapDescriptorAssetManager()
      .load(cubeMapDescriptorSourcePath);

    if (!descriptor)
      throw RuntimeErrorException(
        String::Format(
          "Failed to load CubeMapDescriptor from path '%s'", cubeMapDescriptorSourcePath.string().c_str()
        )
      );

    SharedPtr<Image> rightImage = assetManager.getImageAssetManager().load(
      Path(descriptor->rightImagePath.c_str())
    );
    SharedPtr<Image> leftImage = assetManager.getImageAssetManager().load(
      Path(descriptor->leftImagePath.c_str())
    );
    SharedPtr<Image> topImage = assetManager.getImageAssetManager().load(
      Path(descriptor->topImagePath.c_str())
    );
    SharedPtr<Image> bottomImage = assetManager.getImageAssetManager().load(
      Path(descriptor->bottomImagePath.c_str())
    );
    SharedPtr<Image> backImage = assetManager.getImageAssetManager().load(
      Path(descriptor->backImagePath.c_str())
    );
    SharedPtr<Image> frontImage = assetManager.getImageAssetManager().load(
      Path(descriptor->frontImagePath.c_str())
    );

    SharedPtr<Image> defaultImage = nullptr;
    if (!rightImage || !leftImage || !topImage || !bottomImage || !backImage || !frontImage)
    {
      SizeT numPixels =
        static_cast<SizeT>(descriptor->faceSize)
        * static_cast<SizeT>(descriptor->faceSize);
      SizeT numChannels = textureFormatType::GetChannelCount(descriptor->format);
      SizeT dataSize = numPixels * numChannels;

      BufferByte defaultData(dataSize);

      for (SizeT i = 0; i < dataSize; ++i)
        defaultData[i] = 255;

      defaultImage = MakeShared<Image>(
        Path(),
        descriptor->faceSize,
        descriptor->faceSize,
        descriptor->format,
        colorSpaceType::SRGB,
        std::move(defaultData)
      );
    }

    if (!rightImage)
    {
      rightImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load right image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->rightImagePath.c_str()
        )
      );
    }

    if (!leftImage)
    {
      leftImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load left image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->leftImagePath.c_str()
        )
      );
    }

    if (!topImage)
    {
      topImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load top image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->topImagePath.c_str()
        )
      );
    }

    if (!bottomImage)
    {
      bottomImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load bottom image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->bottomImagePath.c_str()
        )
      );
    }

    if (!backImage)
    {
      backImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load back image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->backImagePath.c_str()
        )
      );
    }

    if (!frontImage)
    {
      frontImage = defaultImage;
      LogService::Warning(
        String::Format(
          "Failed to load front image for CubeMapDescriptor '%s'. Image Path: '%s'. Using default white image.",
          cubeMapDescriptorSourcePath.string().c_str(),
          descriptor->frontImagePath.c_str()
        )
      );
    }

    SharedPtr<ICubeMap> cubeMap = graphicsManager.createCubeMap();
    cubeMap->initialize(
      descriptor->faceSize, descriptor->faceSize,
      *rightImage, *leftImage, *topImage,
      *bottomImage, *backImage, *frontImage,
      cubeMapDescriptorSourcePath
    );

    return cubeMap;
  }
}
