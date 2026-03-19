#include "hc/editor/views/projectFileDialog/hcCreateNewFileUI.h"
#include <imgui.h>

namespace hc::editor
{
  static constexpr const char* CREATE_FILE_ERROR_POPUP_KEY = "CreateFileErrorPopup";

  CreateNewFileUI::CreateNewFileUI() :
    m_targetDirectory(),
    m_fileFilters(),
    m_onFileCreated(nullptr),
    m_newFileName(),
    m_selectedFilterIndex(0),
    m_popupErrorMessage(),
    m_fileCreated(false)
  {
  }

  CreateNewFileUI::~CreateNewFileUI()
  {
  }

  bool CreateNewFileUI::isInitialized() const
  {
    return !m_targetDirectory.empty()
      && !m_fileFilters.empty()
      && m_onFileCreated != nullptr;
  }

  void CreateNewFileUI::initialize(
    const Path& targetDirectory,
    const Vector<String>& filters,
    const std::function<void(const Path&)>& onFileCreated
  )
  {
    reset();
    m_fileCreated = false;
    m_targetDirectory = targetDirectory;
    m_fileFilters = filters;
    m_onFileCreated = onFileCreated;    
  }

  void CreateNewFileUI::setTargetDirectory(const Path& targetDirectory)
  {
    m_targetDirectory = targetDirectory;
  }

  void CreateNewFileUI::draw()
  {
    ImGui::Text("Create New File");
    drawFileNameInput();
    drawFileTypeComboBox();
    drawCreateButton();
    drawPopups();
  }

  bool CreateNewFileUI::wasFileCreated() const
  {
    return m_fileCreated;
  }

  void CreateNewFileUI::reset()
  {
    m_targetDirectory.clear();
    m_fileFilters.clear();
    m_onFileCreated = nullptr;
    m_newFileName.clear();
    m_selectedFilterIndex = 0;
    m_popupErrorMessage.clear();
  }

  void CreateNewFileUI::drawFileNameInput()
  {
    char newFileNameBuffer[256] = { 0 };
    if (ImGui::InputText("New File Name", newFileNameBuffer, sizeof(newFileNameBuffer)))
      m_newFileName = String(newFileNameBuffer);
  }

  void CreateNewFileUI::drawFileTypeComboBox()
  {
    if (m_fileFilters.empty())
      return;

    if (ImGui::BeginCombo("File Type", m_fileFilters[m_selectedFilterIndex].c_str()))
    {
      for (Int32 i = 0; i < static_cast<Int32>(m_fileFilters.size()); ++i)
      {
        bool isSelected = (m_selectedFilterIndex == i);
        if (ImGui::Selectable(m_fileFilters[i].c_str(), isSelected))
          m_selectedFilterIndex = i;

        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  }

  void CreateNewFileUI::drawCreateButton()
  {
    if (ImGui::Button("Create"))
    {
      if (!m_onFileCreated)
      {
        m_popupErrorMessage = "File creation callback is not set.";
        ImGui::OpenPopup(CREATE_FILE_ERROR_POPUP_KEY);
        return;
      }

      if (m_fileFilters.empty())
      {
        m_popupErrorMessage = "No file type filters available.";
        ImGui::OpenPopup(CREATE_FILE_ERROR_POPUP_KEY);
        return;
      }

      if (m_newFileName.empty())
      {
        m_popupErrorMessage = "File name cannot be empty.";
        ImGui::OpenPopup(CREATE_FILE_ERROR_POPUP_KEY);
        return;
      }

      if (m_targetDirectory.empty())
      {
        m_popupErrorMessage = "Target directory is not set.";
        ImGui::OpenPopup(CREATE_FILE_ERROR_POPUP_KEY);
        return;
      }

      // Generate the full file name with extension
      String selectedExtension = m_fileFilters[m_selectedFilterIndex];
      String finalFileName = m_newFileName;
      if (!finalFileName.ends_with(selectedExtension))
        finalFileName += selectedExtension;

      // Create the new file
      Path newFilePath = m_targetDirectory / finalFileName.c_str();

      std::ofstream fileStream(newFilePath);
      if (!fileStream.is_open())
      {
        m_popupErrorMessage = "Failed to create file at: " + newFilePath.string();
        ImGui::OpenPopup(CREATE_FILE_ERROR_POPUP_KEY);
        return;
      }
      fileStream.close();

      m_onFileCreated(newFilePath);
      reset();
      m_fileCreated = true;
    }
  }

  void CreateNewFileUI::drawPopups()
  {
    if (ImGui::BeginPopup(CREATE_FILE_ERROR_POPUP_KEY))
    {
      ImGui::Text("%s", m_popupErrorMessage.c_str());
      if (ImGui::Button("OK"))
        ImGui::CloseCurrentPopup();
      ImGui::EndPopup();
    }
  }
}
