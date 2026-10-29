#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/cubeMap/hcOpenGlCubeMap.h"

namespace hc::graphics
{
  class OpenGlEnvironmentManager
  {
  public:
    OpenGlEnvironmentManager();
    ~OpenGlEnvironmentManager();

    void initialize();
    void setEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap);
    void setDiffuseIrradianceCubeMap(SharedPtr<ICubeMap> cubeMap);
    void setPrefilteredEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap);
    void clearEnvironmentCubeMap();
    void clearDiffuseIrradianceCubeMap();
    void clearPrefilteredEnvironmentCubeMap();
    void bindEnvironmentResources() const;
    inline bool isInitialized() const
    {
      return m_isInitialized;
    }
    void destroy();

  private:
    SharedPtr<ICubeMap> m_environmentCubeMap;
    SharedPtr<ICubeMap> m_diffuseIrradianceCubeMap;
    SharedPtr<ICubeMap> m_prefilteredEnvironmentCubeMap;
    Image m_blackImage;
    OpenGlCubeMap m_blackCubeMap;
    bool m_isInitialized;
  };
}
