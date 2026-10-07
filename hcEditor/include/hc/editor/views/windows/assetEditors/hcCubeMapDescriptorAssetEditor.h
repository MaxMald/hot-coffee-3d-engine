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
    ~CubeMapDescriptorAssetEditor();

    void destroy() override;
    void onProjectOpened() override;
    void onProjectClosed() override;

  private:
    ProjectManager& m_projectManager;
    ProjectFileDialogView& m_fileDialog;
    Vector<String> m_cubeMapDescriptorExtensions;
    Path m_assetPath;
    UInt32 m_faceSize;
    textureFormatType::Type m_format;
    Path m_pXImagePath;
    Path m_nXImagePath;
    Path m_pYImagePath;
    Path m_nYImagePath;
    Path m_pZImagePath;
    Path m_nZImagePath;
    String m_formatStrings[textureFormatType::Count];
    const char* m_formatItems[textureFormatType::Count];

    void onDraw() override;

    void clear();
    bool canSave() const;
    bool canSaveAs() const;
    bool save(const Path& path) const;
    bool load(const Path& path);
  };
}
