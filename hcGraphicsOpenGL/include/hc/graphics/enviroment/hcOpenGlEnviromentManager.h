#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/cubeMap/hcOpenGlCubeMap.h"

namespace hc::graphics
{
  class OpenGlEnviromentManager
  {
  public:
    OpenGlEnviromentManager();
    ~OpenGlEnviromentManager();

    void initialize();
    void setEnviromentCubeMap(SharedPtr<ICubeMap> cubeMap);
    void setDiffuseIrradianceCubeMap(SharedPtr<ICubeMap> cubeMap);
    void clearEnviromentCubeMap();
    void clearDiffuseIrradianceCubeMap();
    void bindEnviromentResources() const;
    inline bool isInitialized() const
    {
      return m_isInitialized;
    }
    void destroy();

  private:
    SharedPtr<ICubeMap> m_enviromentCubeMap;
    SharedPtr<ICubeMap> m_diffuseIrradianceCubeMap;
    Image m_blackImage;
    OpenGlCubeMap m_blackCubeMap;
    bool m_isInitialized;
  };
}
