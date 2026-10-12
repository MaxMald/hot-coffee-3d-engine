#include "hc/graphics/resource/cubeMap/hcCubeMapFactory.h"

#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"
#include "hc/assets/cubeMapDescriptor/hcICubeMapDescriptorAssetManager.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/image/hcIImageAssetManager.h"
#include "hc/graphics/hcIGraphicsManager.h"
#include "hc/graphics/resource/cubeMap/hcICubeMap.h"
#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/generators/hcCubeMapGeneratorSettings.h"
#include "hc/graphics/generators/hcIEquirectangularToCubeMapGenerator.h"

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

    Path pXImagePath = descriptor->pXImagePath;
    Path nXImagePath = descriptor->nXImagePath;
    Path pYImagePath = descriptor->pYImagePath;
    Path nYImagePath = descriptor->nYImagePath;
    Path pZImagePath = descriptor->pZImagePath;
    Path nZImagePath = descriptor->nZImagePath;

    Path rootPath = assetManager.getRootPath();
    bool hasRootPath = assetManager.hasRootPath();

    if (pXImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for positive X face image; asset manager has no root path set.",
            pXImagePath.toString().c_str()
          )
        );

      pXImagePath = pXImagePath.toAbsolute(rootPath);
    }

    if (nXImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for negative X face image; asset manager has no root path set.",
            nXImagePath.toString().c_str()
          )
        );

      nXImagePath = nXImagePath.toAbsolute(rootPath);
    }

    if (pYImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for positive Y face image; asset manager has no root path set.",
            pYImagePath.toString().c_str()
          )
        );
      pYImagePath = pYImagePath.toAbsolute(rootPath);
    }

    if (nYImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for negative Y face image; asset manager has no root path set.",
            nYImagePath.toString().c_str()
          )
        );
      nYImagePath = nYImagePath.toAbsolute(rootPath);
    }

    if (pZImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for positive Z face image; asset manager has no root path set.",
            pZImagePath.toString().c_str()
          )
        );
      pZImagePath = pZImagePath.toAbsolute(rootPath);
    }

    if (nZImagePath.isRelative())
    {
      if (!hasRootPath)
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Cannot resolve relative path '%s' for negative Z face image; asset manager has no root path set.",
            nZImagePath.toString().c_str()
          )
        );
      nZImagePath = nZImagePath.toAbsolute(rootPath);
    }

    IImageAssetManager& imageManager = assetManager.getImageAssetManager();
    SharedPtr<Image> pXImage = imageManager.load(pXImagePath);
    if (pXImage == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load positive X face image from path '%s'.",
          pXImagePath.toString().c_str()
        )
      );

    SharedPtr<Image> nXImage = imageManager.load(nXImagePath);
    if (nXImage == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load negative X face image from path '%s'.",
          nXImagePath.toString().c_str()
        )
      );

    SharedPtr<Image> pYImage = imageManager.load(pYImagePath);
    if (pYImage == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load positive Y face image from path '%s'.",
          pYImagePath.toString().c_str()
        )
      );

    SharedPtr<Image> nYImage = imageManager.load(nYImagePath);
    if (nYImage == nullptr)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load negative Y face image from path '%s'.",
          nYImagePath.toString().c_str()
        )
      );

    SharedPtr<Image> pZImage = imageManager.load(pZImagePath);
    if (pZImage == nullptr)
      throw  RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load positive Z face image from path '%s'.",
          pZImagePath.toString().c_str()
        )
      );

    SharedPtr<Image> nZImage = imageManager.load(nZImagePath);
    if (nZImage == nullptr)
      throw  RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load negative Z face image from path '%s'.",
          nZImagePath.toString().c_str()
        )
      );

    try
    {
      SharedPtr<ICubeMap> cubeMap = graphicsManager.createCubeMap();
      cubeMap->initialize(
        *pXImage,
        *nXImage,
        *pYImage,
        *nYImage,
        *pZImage,
        *nZImage,
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

  SharedPtr<ICubeMap> CubeMapFactory::CreateFromEquirectangularImage(
    const Path& equirectangularImageSourcePath,
    const graphics::generators::CubeMapGeneratorSettings& settings,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  )
  {
    IImageAssetManager& imageManager = assetManager.getImageAssetManager();
    SharedPtr<Image> equirectangularImage = imageManager.load(equirectangularImageSourcePath);
    if (!equirectangularImage)
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to load equirectangular image from path '%s'.",
          equirectangularImageSourcePath.toString().c_str()
        )
      );

    try
    {
      SharedPtr<ITexture> equirectangularTexture = graphicsManager
        .getTextureManager()
        .createTextureFromImage(equirectangularImage);

      if (equirectangularTexture == nullptr || !equirectangularTexture->isValid())
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Failed to create texture from equirectangular image '%s'.",
            equirectangularImageSourcePath.toString().c_str()
          )
        );

      SharedPtr<ICubeMap> cubeMap = graphicsManager
        .getEquirectangularToCubeMapGenerator()
        .generate(*equirectangularTexture, settings);

      if (cubeMap == nullptr || !cubeMap->isValid())
        throw RuntimeErrorException(
          String::Format(
            "CubeMapFactory: Failed to generate cube map from equirectangular image '%s'.",
            equirectangularImageSourcePath.toString().c_str()
          )
        );

      return cubeMap;
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        String::Format(
          "CubeMapFactory: Failed to create cube map from equirectangular image '%s': %s",
          equirectangularImageSourcePath.toString().c_str(),
          e.what()
        )
      );
    }
  }
}
