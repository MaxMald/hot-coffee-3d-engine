#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class EditorSceneManager;
  class ProjectFileDialogView;

  class SceneSettingsWindow : public AWindowView
  {
  public:
    SceneSettingsWindow(
      EditorSceneManager& editorSceneManager,
      ProjectFileDialogView& projectFileDialogView,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
    ~SceneSettingsWindow() override;

    virtual void destroy() override;

  protected:
    virtual void onDraw() override;

  private:
    EditorSceneManager& m_editorSceneManager;
    ProjectFileDialogView& m_projectFileDialogView;
    IAssetManager& m_assetManager;
    IGraphicsManager& m_graphicsManager;
    Vector<String> m_cubeMapDescriptorExtensions;

    void updateSkyboxCubeMap(const Path& cubeMapDescriptorPath);
  };
}
