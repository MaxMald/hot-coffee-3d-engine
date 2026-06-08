#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"

namespace hc::editor
{
  class ProjectManager;
  class ProjectFileDialogView;

  class CubeMapDescriptorAssetEditor :
    public AWindowView,
    public IProjectManagerListener
  {
  public:
    CubeMapDescriptorAssetEditor(
      ProjectManager& projectManager,
      ProjectFileDialogView& fileDialog
    );
    ~CubeMapDescriptorAssetEditor() = default;

    void onProjectOpened() override;
    void onProjectClosed() override;

  private:
    ProjectManager& m_projectManager;
    ProjectFileDialogView& m_fileDialog;
    Vector<String> m_cubeMapDescriptorExtensions;
    Path m_assetPath;
    bool m_useRelativePaths;
    UInt32 m_faceSize;
    colorFormatType::Type m_format;
    Path m_rightImagePath;
    Path m_leftImagePath;
    Path m_topImagePath;
    Path m_bottomImagePath;
    Path m_backImagePath;
    Path m_frontImagePath;
    String m_formatStrings[colorFormatType::Count];
    const char* m_formatItems[colorFormatType::Count];

    void onDraw() override;

    void clear();
    bool canSave() const;
    bool canSaveAs() const;
    bool save(const Path& path) const;
    bool load(const Path& path);
  };
}
