#include "hc/editor/views/windows/gameObjectEditor/hcCreateComponentSection.h"

#include <imgui.h>
#include "hc/editor/hcEditorCommons.h"

namespace hc::editor
{
  CreateComponentSection::CreateComponentSection() :
    m_selectedComponentTypeIndex(0),
    m_userRequestedCreation(false)
  {
  }

  CreateComponentSection::~CreateComponentSection()
  {
  }

  void CreateComponentSection::draw()
  {
    m_userRequestedCreation = false;

    ImGui::Text("Create Component:");

    ImGui::SetNextItemWidth(120.0f);
    ImGui::Combo("Selected Type", &m_selectedComponentTypeIndex, comboItems::COMPONENT_TYPES, comboItems::COMPONENT_TYPES_COUNT);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::Button("Create"))
      m_userRequestedCreation = true;
  }

  bool CreateComponentSection::wasCreationRequested() const
  {
    return m_userRequestedCreation;
  }

  void CreateComponentSection::createComponentFromSelection(
    GameObject* gameObject
  )
  {
    if (!gameObject)
    {
      LogService::Error("Game Object is null. Cannot create component.");
      m_userRequestedCreation = false;
      return;
    }

    try
    {
      componentType::Type selectedType
        = static_cast<componentType::Type>(m_selectedComponentTypeIndex);

      switch (selectedType)
      {
      case componentType::Type::Mesh:
        createComponent<MeshComponent>(gameObject);
        break;
      case componentType::Type::Camera:
        createComponent<CameraComponent>(gameObject);
        break;
      case componentType::Type::Light:
        createComponent<LightComponent>(gameObject);
        break;
      default:
        LogService::Error(
          String::Format(
            "CreateComponentSection::createComponentFromSelection: "
            "Unknown component type selected (%s).",
            comboItems::COMPONENT_TYPES[m_selectedComponentTypeIndex]
          )
        );
        break;
      }
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to create component of type %s. Error: %s",
          comboItems::COMPONENT_TYPES[m_selectedComponentTypeIndex],
          e.what()
        )
      );
    }

    m_userRequestedCreation = false;
  }
}
