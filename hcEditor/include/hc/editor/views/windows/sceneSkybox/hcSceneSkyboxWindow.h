#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class EditorSceneManager;

  class SceneSkyboxWindow : public AWindowView
  {
  public:
    SceneSkyboxWindow(
      EditorSceneManager& editorSceneManager,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
    ~SceneSkyboxWindow() override;

  private:
    EditorSceneManager& m_editorSceneManager;
    IAssetManager& m_assetManager;
    IGraphicsManager& m_graphicsManager;

    UInt32 m_faceWidth;
    UInt32 m_faceHeight;
    UInt8 m_faceChannels;
    Path m_rightImagePath;
    Path m_leftImagePath;
    Path m_topImagePath;
    Path m_bottomImagePath;
    Path m_backImagePath;
    Path m_frontImagePath;

    void onDraw() override;
    bool updateSkybox();
    void getValuesFromScene(const Scene& scene);
    void clearValues();
  };
}
