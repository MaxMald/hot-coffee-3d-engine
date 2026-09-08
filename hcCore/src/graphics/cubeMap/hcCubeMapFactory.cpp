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
          "Failed to load CubeMapDescriptor from path '%s'", cubeMapDescriptorSourcePath.toString().c_str()
        )
      );

    SharedPtr<ICubeMap> cubeMap = graphicsManager.createCubeMap();
    cubeMap->initialize(
      descriptor,
      assetManager
    );

    return cubeMap;
  }
}
