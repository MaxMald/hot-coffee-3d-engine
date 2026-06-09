#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class EditorSceneManager;
  class ProjectFileDialogView;

  class SceneSkyboxWindow : public AWindowView
  {
  public:
    SceneSkyboxWindow(
      EditorSceneManager& editorSceneManager,
      ProjectFileDialogView& projectFileDialogView,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
    ~SceneSkyboxWindow() override;

    void destroy() override;

  private:
    EditorSceneManager& m_editorSceneManager;
    ProjectFileDialogView& m_projectFileDialogView;
    IAssetManager& m_assetManager;
    IGraphicsManager& m_graphicsManager;
    Vector<String> m_cubeMapDescriptorExtensions;

    void onDraw() override;
    void updateSkyboxCubeMap(const Path& cubeMapDescriptorPath);
  };
}
