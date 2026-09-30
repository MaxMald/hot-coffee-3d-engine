#include "hc/graphics/cubeMap/hcCubeMapFactory.h"

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
          "Failed to load CubeMapDescriptor from path '%s'", cubeMapDescriptorSourcePath.toString().c_str()
        )
      );

    Path rightFacePath = descriptor->rightImagePath;
    Path leftFacePath = descriptor->leftImagePath;
    Path topFacePath = descriptor->topImagePath;
    Path bottomFacePath = descriptor->bottomImagePath;
    Path backFacePath = descriptor->backImagePath;
    Path frontFacePath = descriptor->frontImagePath;

    Path rootPath = assetManager.getRootPath();
    bool hasRootPath = assetManager.hasRootPath();

    if (rightFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for right face image; asset manager has no root path set.",
            rightFacePath.toString().c_str()
          )
        );

      rightFacePath = rightFacePath.toAbsolute(rootPath);
    }

    if (leftFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for left face image; asset manager has no root path set.",
            leftFacePath.toString().c_str()
          )
        );

      leftFacePath = leftFacePath.toAbsolute(rootPath);
    }

    if (topFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for top face image; asset manager has no root path set.",
            topFacePath.toString().c_str()
          )
        );
      topFacePath = topFacePath.toAbsolute(rootPath);
    }

    if (bottomFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for bottom face image; asset manager has no root path set.",
            bottomFacePath.toString().c_str()
          )
        );
      bottomFacePath = bottomFacePath.toAbsolute(rootPath);
    }

    if (backFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for back face image; asset manager has no root path set.",
            backFacePath.toString().c_str()
          )
        );
      backFacePath = backFacePath.toAbsolute(rootPath);
    }

    if (frontFacePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for front face image; asset manager has no root path set.",
            frontFacePath.toString().c_str()
          )
        );
      frontFacePath = frontFacePath.toAbsolute(rootPath);
    }

    IImageAssetManager& imageManager = assetManager.getImageAssetManager();
    SharedPtr<Image> rightFace = imageManager.load(rightFacePath);
    if (rightFace == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load right face image from path '%s'.",
          rightFacePath.toString().c_str()
        )
      );

    SharedPtr<Image> leftFace = imageManager.load(leftFacePath);
    if (leftFace == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load left face image from path '%s'.",
          leftFacePath.toString().c_str()
        )
      );

    SharedPtr<Image> topFace = imageManager.load(topFacePath);
    if (topFace == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load top face image from path '%s'.",
          topFacePath.toString().c_str()
        )
      );

    SharedPtr<Image> bottomFace = imageManager.load(bottomFacePath);
    if (bottomFace == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load bottom face image from path '%s'.",
          bottomFacePath.toString().c_str()
        )
      );

    SharedPtr<Image> backFace = imageManager.load(backFacePath);
    if (backFace == nullptr)
      throw  RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load back face image from path '%s'.",
          backFacePath.toString().c_str()
        )
      );

    SharedPtr<Image> frontFace = imageManager.load(frontFacePath);
    if (frontFace == nullptr)
      throw  RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load front face image from path '%s'.",
          frontFacePath.toString().c_str()
        )
      );

    try
    {
      SharedPtr<ICubeMap> cubeMap = graphicsManager.createCubeMap();
      cubeMap->initialize(
        *rightFace,
        *leftFace,
        *topFace,
        *bottomFace,
        *backFace,
        *frontFace,
        cubeMapDescriptorSourcePath
      );
      return cubeMap;
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to create cube map from descriptor '%s': %s",
          cubeMapDescriptorSourcePath.toString().c_str(),
          e.what()
        )
      );
    }
  }
}
