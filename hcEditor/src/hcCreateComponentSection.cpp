#include "hc/editor/hcCreateComponentSection.h"
#include "imgui.h"

namespace
{
  constexpr const hc::Char* COMPONENT_TYPES[] =
  {
    "Mesh"
  };
}

namespace hc::editor
{
  CreateComponentSection::CreateComponentSection() :
    m_selectedComponentTypeIndex(0),
    m_userRequestedCreation(false)
  {
  }

  CreateComponentSection::~CreateComponentSection() = default;

  void CreateComponentSection::draw()
  {
    m_userRequestedCreation = false;

    ImGui::Text("Create Component:");
    
    ImGui::SetNextItemWidth(120.0f);
    ImGui::Combo("Selected Type", &m_selectedComponentTypeIndex, COMPONENT_TYPES, IM_ARRAYSIZE(COMPONENT_TYPES));
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::Button("Create"))
      m_userRequestedCreation = true;
  }

  bool CreateComponentSection::wasCreationRequested() const
  {
    return m_userRequestedCreation;
  }

  UniquePtr<IComponent> CreateComponentSection::createComponentFromSelection()
  {
    componentType::Type selectedType = componentType::FromString(
      COMPONENT_TYPES[m_selectedComponentTypeIndex]
    );

    switch (selectedType)
    {
    case componentType::Type::Mesh:
      return MakeUnique<MeshComponent>();

    default:
    {
      LogService::Error(
        String::Format(
          "CreateComponentSection::createComponentFromSelection: "
          "Unknown component type selected (%s).",
          COMPONENT_TYPES[m_selectedComponentTypeIndex]
        )
      );
      break;
    } 
    }

    m_userRequestedCreation = false;
    return nullptr;
  }
}
