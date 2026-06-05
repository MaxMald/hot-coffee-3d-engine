#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class MeshShapeFactory
  {
  public:
    static SharedPtr<IMesh> CreateCircle(
      IMeshManager& meshManager,
      float radius,
      UInt32 segments,
      const Color& color = Color::White()
    );

    static SharedPtr<IMesh> CreateLine(
      IMeshManager& meshManager,
      const Vector3f& start,
      const Vector3f& end,
      const Color& color = Color::White()
    );

    static SharedPtr<IMesh> CreateRectangle(
      IMeshManager& meshManager,
      float width,
      float height,
      const Color& color = Color::White()
    );

    static SharedPtr<IMesh> CreateBox(
      IMeshManager& meshManager,
      float width,
      float height,
      float depth,
      const Color& color = Color::White()
    );
  };
}
