#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlCubeMap : public ICubeMap
  {
  public:
    OpenGlCubeMap();
    ~OpenGlCubeMap() override;

    void initialize(
      SharedPtr<CubeMapDescriptor> cubeMapDescriptor,
      IAssetManager& assetManager
    ) override;

    bool isValid() const override;
    UInt32 getFaceWidth() const override;
    UInt32 getFaceHeight() const override;
    SharedPtr<CubeMapDescriptor> getCubeMapDescriptor() const override;
    void destroy() override;

    UInt32 getId() const;

  private:
    UInt32 m_id;
    bool m_valid;
    SharedPtr<CubeMapDescriptor> m_descriptor;

    static void assertImageSize(
      const Image& image,
      const UInt32 width,
      const UInt32 height
    );

    void assertIsValid() const;
  };
}
