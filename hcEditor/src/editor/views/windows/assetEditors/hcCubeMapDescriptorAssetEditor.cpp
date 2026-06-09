#include "hc/editor/views/windows/assetEditors/hcCubeMapDescriptorAssetEditor.h"

#include <fstream>
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
    m_format(colorFormatType::RGBA8),
    m_rightImagePath(),
    m_leftImagePath(),
    m_topImagePath(),
    m_bottomImagePath(),
    m_backImagePath(),
    m_frontImagePath(),
    m_formatStrings(),
    m_formatItems()
  {
    for (UInt8 i = 0; i < colorFormatType::Count; ++i)
    {
      m_formatStrings[i] = colorFormatType::ToString(static_cast<colorFormatType::Type>(i));
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

    ImGui::LabelText("Asset Path", "%s", m_assetPath.empty() ? "New Asset" : m_assetPath.generic_string().c_str());

    // Input fields for face dimensions and channels
    Int32 faceSizeInput = static_cast<Int32>(m_faceSize);
    if (ImGui::InputInt("Face Size", &faceSizeInput) && faceSizeInput > 0)
      m_faceSize = static_cast<UInt32>(faceSizeInput);

    Int32 selectedFormat = static_cast<Int32>(m_format);
    if (ImGui::Combo("Format", &selectedFormat, m_formatItems, colorFormatType::Count))
      m_format = static_cast<colorFormatType::Type>(selectedFormat);

    // Input fields for each cube map face

    if (ImGui::Button("Select Right Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_rightImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Right Image", "%s", m_rightImagePath.empty() ? "No file selected" : m_rightImagePath.generic_string().c_str());

    if (ImGui::Button("Select Left Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_leftImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Left Image", "%s", m_leftImagePath.empty() ? "No file selected" : m_leftImagePath.generic_string().c_str());

    if (ImGui::Button("Select Top Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_topImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Top Image", "%s", m_topImagePath.empty() ? "No file selected" : m_topImagePath.generic_string().c_str());

    if (ImGui::Button("Select Bottom Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_bottomImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Bottom Image", "%s", m_bottomImagePath.empty() ? "No file selected" : m_bottomImagePath.generic_string().c_str());

    if (ImGui::Button("Select Back Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_backImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Back Image", "%s", m_backImagePath.empty() ? "No file selected" : m_backImagePath.generic_string().c_str());

    if (ImGui::Button("Select Front Image"))
      m_fileDialog.openImageFile([this](const Path& path) { m_frontImagePath = path; });
    ImGui::SameLine();
    ImGui::LabelText("Front Image", "%s", m_frontImagePath.empty() ? "No file selected" : m_frontImagePath.generic_string().c_str());

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
    m_format = colorFormatType::RGBA8;
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

      Path baseDir = path.parent_path();
      descriptorToSave.rightImagePath = AssetPath::ToRelative(m_rightImagePath, baseDir);
      descriptorToSave.leftImagePath = AssetPath::ToRelative(m_leftImagePath, baseDir);
      descriptorToSave.topImagePath = AssetPath::ToRelative(m_topImagePath, baseDir);
      descriptorToSave.bottomImagePath = AssetPath::ToRelative(m_bottomImagePath, baseDir);
      descriptorToSave.backImagePath = AssetPath::ToRelative(m_backImagePath, baseDir);
      descriptorToSave.frontImagePath = AssetPath::ToRelative(m_frontImagePath, baseDir);

      std::ofstream file(path, std::ios::out | std::ios::binary);
      if (!file.is_open())
        throw IOException("Failed to open file for writing: " + path.generic_string());

      BinaryWriter writer(file);
      descriptorToSave.serialize(writer);
    }
    catch (const Exception& e)
    {
      LogService::Error("Exception while saving CubeMapDescriptor: " + String(e.what()));
      return false;
    }

    LogService::Message("Successfully saved cube map descriptor: " + path.generic_string());
    return true;
  }

  bool CubeMapDescriptorAssetEditor::load(const Path& path)
  {
    try
    {
      clear();

      std::ifstream file(path, std::ios::in | std::ios::binary);
      if (!file.is_open())
        throw IOException("Failed to open file for reading: " + path.generic_string());


      BinaryReader reader(file);
      CubeMapDescriptor descriptorFromFile;
      descriptorFromFile.deserialize(reader);

      m_faceSize = descriptorFromFile.faceSize;
      m_format = descriptorFromFile.format;

      Path baseDir = path.parent_path();
      m_rightImagePath = AssetPath::ToAbsolute(descriptorFromFile.rightImagePath, baseDir);
      m_leftImagePath = AssetPath::ToAbsolute(descriptorFromFile.leftImagePath, baseDir);
      m_topImagePath = AssetPath::ToAbsolute(descriptorFromFile.topImagePath, baseDir);
      m_bottomImagePath = AssetPath::ToAbsolute(descriptorFromFile.bottomImagePath, baseDir);
      m_backImagePath = AssetPath::ToAbsolute(descriptorFromFile.backImagePath, baseDir);
      m_frontImagePath = AssetPath::ToAbsolute(descriptorFromFile.frontImagePath, baseDir);
      m_assetPath = path;
    }
    catch (const Exception & e)
    {
      clear();
      LogService::Error("Exception while loading CubeMapDescriptor: " + String(e.what()));
      return false;
    }

    LogService::Message("Successfully loaded cube map descriptor: " + path.generic_string());
    return true;
  }
}
