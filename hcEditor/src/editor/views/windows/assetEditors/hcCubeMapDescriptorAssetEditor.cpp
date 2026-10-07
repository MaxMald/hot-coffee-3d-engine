#include "hc/editor/views/windows/assetEditors/hcCubeMapDescriptorAssetEditor.h"

#include "hc/editor/imgui/hcImgui.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  CubeMapDescriptorAssetEditor::CubeMapDescriptorAssetEditor(
    ProjectManager& projectManager,
    ProjectFileDialogView& fileDialog
  ) :
    AWindowView("Cube Map Descriptor Editor", false, Vector2f(250, 250)),
    m_projectManager(projectManager),
    m_fileDialog(fileDialog),
    m_cubeMapDescriptorExtensions({ hc::serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION }),
    m_assetPath(),
    m_faceSize(0),
    m_format(textureFormatType::RGBA8),
    m_pXImagePath(),
    m_nXImagePath(),
    m_pYImagePath(),
    m_nYImagePath(),
    m_pZImagePath(),
    m_nZImagePath(),
    m_formatStrings(),
    m_formatItems()
  {
    for (UInt8 i = 0; i < textureFormatType::Count; ++i)
    {
      m_formatStrings[i] = textureFormatType::ToString(static_cast<textureFormatType::Type>(i));
      m_formatItems[i] = m_formatStrings[i].c_str();
    }

    m_projectManager.subscribeListener(this);
  }

  CubeMapDescriptorAssetEditor::~CubeMapDescriptorAssetEditor()
  {
    destroy();
  }

  void CubeMapDescriptorAssetEditor::destroy()
  {
    clear();
    m_projectManager.unsubscribeListener(this);
  }

  void CubeMapDescriptorAssetEditor::onProjectOpened()
  {
    clear();
  }

  void CubeMapDescriptorAssetEditor::onProjectClosed()
  {
    clear();
  }

  void CubeMapDescriptorAssetEditor::onDraw()
  {
    if (!m_projectManager.isProjectOpen())
    {
      ImGui::Text("No project open. Please open a project to edit cube map descriptors.");
      return;
    }

    ImGui::LabelText("Asset Path", "%s", m_assetPath.empty() ? "New Asset" : m_assetPath.toGenericString().c_str());

    // Input fields for face dimensions and channels
    Int32 faceSizeInput = static_cast<Int32>(m_faceSize);
    if (ImGui::InputInt("Face Size", &faceSizeInput) && faceSizeInput > 0)
      m_faceSize = static_cast<UInt32>(faceSizeInput);

    Int32 selectedFormat = static_cast<Int32>(m_format);
    if (ImGui::Combo("Format", &selectedFormat, m_formatItems, textureFormatType::Count))
      m_format = static_cast<textureFormatType::Type>(selectedFormat);

    // Input fields for each cube map face

    if (ImGui::Button("Select Positive X Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_pXImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Positive X Image", m_pXImagePath);

    if (ImGui::Button("Select Negative X Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_nXImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Negative X Image", m_nXImagePath);

    if (ImGui::Button("Select Positive Y Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_pYImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Positive Y Image", m_pYImagePath);

    if (ImGui::Button("Select Negative Y Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_nYImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Negative Y Image", m_nYImagePath);

    if (ImGui::Button("Select Positive Z Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_pZImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Positive Z Image", m_pZImagePath);

    if (ImGui::Button("Select Negative Z Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_nZImagePath = path; });
    ImGui::SameLine();
    hcImGui::Filename("Negative Z Image", m_nZImagePath);

    // Action buttons

    if (ImGui::Button("Clear"))
      clear();

    ImGui::SameLine();

    ImGui::BeginDisabled(!canSave());
    if (ImGui::Button("Save"))
    {
      if (save(m_assetPath))
      {
        clear();
        setOpen(false);
      }
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(!canSaveAs());
    if (ImGui::Button("Save As"))
    {
      m_fileDialog.openFileSelector(
        "Save Cube Map Descriptor",
        m_cubeMapDescriptorExtensions,
        [this](const Path& path)
        {
          if (save(path))
          {
            clear();
            setOpen(false);
          }
        },
        []() {},
        true
      );
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
      m_fileDialog.openFileSelector(
        "Load Cube Map Descriptor",
        m_cubeMapDescriptorExtensions,
        [this](const Path& path) { load(path); }
      );
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
      clear();
      setOpen(false);
    }
  }

  void CubeMapDescriptorAssetEditor::clear()
  {
    m_assetPath.clear();
    m_faceSize = 0;
    m_format = textureFormatType::RGBA8;
    m_pXImagePath.clear();
    m_nXImagePath.clear();
    m_pYImagePath.clear();
    m_nYImagePath.clear();
    m_pZImagePath.clear();
    m_nZImagePath.clear();
  }

  bool CubeMapDescriptorAssetEditor::canSave() const
  {
    return !m_assetPath.empty() &&
      !m_pXImagePath.empty() &&
      !m_nXImagePath.empty() &&
      !m_pYImagePath.empty() &&
      !m_nYImagePath.empty() &&
      !m_pZImagePath.empty() &&
      !m_nZImagePath.empty() &&
      m_faceSize > 0;
  }

  bool CubeMapDescriptorAssetEditor::canSaveAs() const
  {
    return !m_pXImagePath.empty() &&
      !m_nXImagePath.empty() &&
      !m_pYImagePath.empty() &&
      !m_nYImagePath.empty() &&
      !m_pZImagePath.empty() &&
      !m_nZImagePath.empty() &&
      m_faceSize > 0;
  }

  bool CubeMapDescriptorAssetEditor::save(const Path& path) const
  {
    try
    {
      CubeMapDescriptor descriptorToSave;

      descriptorToSave.faceSize = m_faceSize;
      descriptorToSave.format = m_format;

      Path baseDir = path.parentPath();
      descriptorToSave.pXImagePath = m_pXImagePath.toRelative(baseDir);
      descriptorToSave.nXImagePath = m_nXImagePath.toRelative(baseDir);
      descriptorToSave.pYImagePath = m_pYImagePath.toRelative(baseDir);
      descriptorToSave.nYImagePath = m_nYImagePath.toRelative(baseDir);
      descriptorToSave.pZImagePath = m_pZImagePath.toRelative(baseDir);
      descriptorToSave.nZImagePath = m_nZImagePath.toRelative(baseDir);

      String error;
      io::BinaryWriter writer;
      if (!writer.prepare(path, error))
        throw IOException("Failed to prepare binary writer: " + error);

      descriptorToSave.serialize(writer);
    }
    catch (const Exception& e)
    {
      LogService::Error("Exception while saving CubeMapDescriptor: " + String(e.what()));
      return false;
    }

    LogService::Message("Successfully saved cube map descriptor: " + path.toGenericString());
    return true;
  }

  bool CubeMapDescriptorAssetEditor::load(const Path& path)
  {
    try
    {
      clear();

      String error;
      io::BinaryReader reader;
      if (!reader.prepare(path, error))
        throw IOException("Failed to prepare binary reader: " + error);

      CubeMapDescriptor descriptorFromFile;
      descriptorFromFile.deserialize(reader);

      m_faceSize = descriptorFromFile.faceSize;
      m_format = descriptorFromFile.format;

      Path baseDir = path.parentPath();
      m_pXImagePath = descriptorFromFile.pXImagePath.toAbsolute(baseDir);
      m_nXImagePath = descriptorFromFile.nXImagePath.toAbsolute(baseDir);
      m_pYImagePath = descriptorFromFile.pYImagePath.toAbsolute(baseDir);
      m_nYImagePath = descriptorFromFile.nYImagePath.toAbsolute(baseDir);
      m_pZImagePath = descriptorFromFile.pZImagePath.toAbsolute(baseDir);
      m_nZImagePath = descriptorFromFile.nZImagePath.toAbsolute(baseDir);
      m_assetPath = path;
    }
    catch (const Exception & e)
    {
      clear();
      LogService::Error("Exception while loading CubeMapDescriptor: " + String(e.what()));
      return false;
    }

    LogService::Message("Successfully loaded cube map descriptor: " + path.toGenericString());
    return true;
  }
}
