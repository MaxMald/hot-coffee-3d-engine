#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/cubeMap/hcOpenGlCubeMap.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc::graphics
{
  class OpenGlEnvironmentManager
  {
  public:
    OpenGlEnvironmentManager(IDataBlockManager& dataBlockManager);
    ~OpenGlEnvironmentManager();

    void initialize(SharedPtr<IShaderProgram> brdfLutGeneratorShaderProgram);
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
    IDataBlockManager& m_dataBlockManager;
    SharedPtr<ICubeMap> m_environmentCubeMap;
    SharedPtr<ICubeMap> m_diffuseIrradianceCubeMap;
    SharedPtr<ICubeMap> m_prefilteredEnvironmentCubeMap;
    SharedPtr<OpenGlTexture> m_brdfLutTexture;
    Image m_blackImage;
    OpenGlCubeMap m_blackCubeMap;
    UInt32 m_fbo;
    UInt32 m_rbo;
    bool m_isInitialized;

    void generateBrdfLutTexture(SharedPtr<IShaderProgram> brdfLutGeneratorShaderProgram);
  };
}
