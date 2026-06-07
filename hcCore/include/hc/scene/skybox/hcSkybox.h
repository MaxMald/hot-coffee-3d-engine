#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ICubeMap;
  class Image;

  class HC_CORE_EXPORT Skybox : public NonCopyable
  {
  public:
    Skybox(SharedPtr<ICubeMap> cubeMap);
    ~Skybox();

    void initialize(
      const UInt32 width,
      const UInt32 height,
      const UInt8 channels,
      const Image& right,
      const Image& left,
      const Image& top,
      const Image& bottom,
      const Image& back,
      const Image& front
    );
    bool isValid() const;
    const ICubeMap& getCubeMap() const;
    const Path& getRightImagePath() const;
    const Path& getLeftImagePath() const;
    const Path& getTopImagePath() const;
    const Path& getBottomImagePath() const;
    const Path& getBackImagePath() const;
    const Path& getFrontImagePath() const;
    void destroy();

  protected:
    SharedPtr<ICubeMap> m_cubeMap;
    Path m_rightImagePath;
    Path m_leftImagePath;
    Path m_topImagePath;
    Path m_bottomImagePath;
    Path m_backImagePath;
    Path m_frontImagePath;
  };
}
