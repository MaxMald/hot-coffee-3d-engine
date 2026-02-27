#include "hc/graphics/resource/material/hcUnlitMaterialFactory.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"
#include "hc/graphics/resource/material/hcUnlitMaterial.h"
#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgramManager.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"

namespace hc
{
  UnlitMaterialFactory::UnlitMaterialFactory()
  {
  }

  SharedPtr<IMaterial> UnlitMaterialFactory::create(
    SharedPtr<AMaterialDescriptor> descriptor,
    ITextureManager& textureManager,
    IShaderProgramManager& shaderProgramManager
  )
  {
    SharedPtr<UnlitMaterialDescriptor> unlitDescriptor =
      std::static_pointer_cast<UnlitMaterialDescriptor>(descriptor);

    if (!unlitDescriptor)
    {
      LogService::Error(String::Format("Invalid UnlitMaterialDescriptor provided."));
      return nullptr;
    }

    SharedPtr<ITexture> mainTexture = nullptr;
    if (!unlitDescriptor->getMainImagePath().empty())
    {
      mainTexture = textureManager.createTextureFromFile(
        unlitDescriptor->getMainImagePath()
      );
    }

    SharedPtr<UnlitMaterial> material = MakeShared<UnlitMaterial>();
    material->initialize(
      shaderProgramManager.getUnlitShaderProgram(),
      unlitDescriptor,
      mainTexture
    );

    return material;
  }
}
