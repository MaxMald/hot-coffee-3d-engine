#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ICubeMap;
  class Image;

  class HC_CORE_EXPORT Skybox : public NonCopyable
  {
  public:
    Skybox();
    ~Skybox();
    void initialize(SharedPtr<ICubeMap> cubeMap);
    bool isValid() const;
    bool hasCubeMap() const;
    const ICubeMap& getCubeMap() const;
    void destroy();

  protected:
    SharedPtr<ICubeMap> m_cubeMap;
  };
}
