#include "hc/editor/views/windows/assetEditors/hcCubeMapDescriptorAssetEditor.h"

#include <imgui.h>
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
    m_rightImagePath(),
    m_leftImagePath(),
    m_topImagePath(),
    m_bottomImagePath(),
    m_backImagePath(),
    m_frontImagePath(),
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

    if (ImGui::Button("Select Right Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_rightImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Right Image", "%s", m_rightImagePath.empty() ? "No file selected" : m_rightImagePath.toGenericString().c_str());

    if (ImGui::Button("Select Left Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_leftImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Left Image", "%s", m_leftImagePath.empty() ? "No file selected" : m_leftImagePath.toGenericString().c_str());

    if (ImGui::Button("Select Top Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_topImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Top Image", "%s", m_topImagePath.empty() ? "No file selected" : m_topImagePath.toGenericString().c_str());

    if (ImGui::Button("Select Bottom Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_bottomImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Bottom Image", "%s", m_bottomImagePath.empty() ? "No file selected" : m_bottomImagePath.toGenericString().c_str());

    if (ImGui::Button("Select Back Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_backImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Back Image", "%s", m_backImagePath.empty() ? "No file selected" : m_backImagePath.toGenericString().c_str());

    if (ImGui::Button("Select Front Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_frontImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Front Image", "%s", m_frontImagePath.empty() ? "No file selected" : m_frontImagePath.toGenericString().c_str());

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
        [this](const Path& path) { load(path); },
        false
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
    m_rightImagePath.clear();
    m_leftImagePath.clear();
    m_topImagePath.clear();
    m_bottomImagePath.clear();
    m_backImagePath.clear();
    m_frontImagePath.clear();
  }

  bool CubeMapDescriptorAssetEditor::canSave() const
  {
    return !m_assetPath.empty() &&
      !m_rightImagePath.empty() &&
      !m_leftImagePath.empty() &&
      !m_topImagePath.empty() &&
      !m_bottomImagePath.empty() &&
      !m_backImagePath.empty() &&
      !m_frontImagePath.empty() &&
      m_faceSize > 0;
  }

  bool CubeMapDescriptorAssetEditor::canSaveAs() const
  {
    return !m_rightImagePath.empty() &&
      !m_leftImagePath.empty() &&
      !m_topImagePath.empty() &&
      !m_bottomImagePath.empty() &&
      !m_backImagePath.empty() &&
      !m_frontImagePath.empty() &&
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
      descriptorToSave.rightImagePath = m_rightImagePath.toRelative(baseDir);
      descriptorToSave.leftImagePath = m_leftImagePath.toRelative(baseDir);
      descriptorToSave.topImagePath = m_topImagePath.toRelative(baseDir);
      descriptorToSave.bottomImagePath = m_bottomImagePath.toRelative(baseDir);
      descriptorToSave.backImagePath = m_backImagePath.toRelative(baseDir);
      descriptorToSave.frontImagePath = m_frontImagePath.toRelative(baseDir);

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
      m_rightImagePath = descriptorFromFile.rightImagePath.toAbsolute(baseDir);
      m_leftImagePath = descriptorFromFile.leftImagePath.toAbsolute(baseDir);
      m_topImagePath = descriptorFromFile.topImagePath.toAbsolute(baseDir);
      m_bottomImagePath = descriptorFromFile.bottomImagePath.toAbsolute(baseDir);
      m_backImagePath = descriptorFromFile.backImagePath.toAbsolute(baseDir);
      m_frontImagePath = descriptorFromFile.frontImagePath.toAbsolute(baseDir);
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
