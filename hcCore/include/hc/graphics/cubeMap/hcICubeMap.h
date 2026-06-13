#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/utilities/hcColorFormatType.h"

namespace hc
{
  class Image;

  class HC_CORE_EXPORT ICubeMap : public NonCopyable
  {
  public:
    virtual ~ICubeMap();

    virtual void initialize(
      const UInt32 width,
      const UInt32 height,
      const Image& right,
      const Image& left,
      const Image& top,
      const Image& bottom,
      const Image& back,
      const Image& front,
      const Path& cubeMapDescriptorSourcePath = Path()
    ) = 0;

    virtual bool isValid() const = 0;
    virtual UInt32 getFaceWidth() const = 0;
    virtual UInt32 getFaceHeight() const = 0;

    /**
     * Get the file path of the cube map descriptor source used to create this skybox.
     * This path is used for serialization and deserialization purposes.
     *
     * @return The file path of the cube map descriptor source.
     */
    virtual const Path& getCubeMapDescriptorSourcePath() const = 0;

    virtual void destroy() = 0;

  protected:
    ICubeMap();
  };
}
