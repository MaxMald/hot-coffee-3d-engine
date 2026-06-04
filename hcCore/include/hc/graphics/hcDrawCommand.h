#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcIMaterial.h"
#include "hc/graphics/resource/material/hcMaterialRenderMode.h"
#include "hc/graphics/hcPolygonFillType.h"

namespace hc
{
  /**
   * OpenGL-specific rendering data for draw commands.
   *
   * Contains the graphics API-specific information needed to execute
   * a draw call in OpenGL.
   */
  struct OpenGlDrawData
  {
    UInt32 vao = 0;
    UInt32 drawMode = 0;
  };

  /**
   * Represents a single draw operation to be executed by the renderer.
   *
   * Draw commands are submitted during the frame and later sorted by
   * render queue, material, and distance before execution.
   */
  struct HC_CORE_EXPORT DrawCommand
  {
    /**
     * The model-to-world transformation matrix.
     */
    Matrix4 modelMatrix = Matrix4::Identity();

    /**
     * The material to use for rendering.
     */
    SharedPtr<IMaterial> material = nullptr;

    /**
     * Packed 64-bit key used for fast sorting of draw commands.
     * Contains render queue, material ID, distance, and other data.
     */
    UInt64 sortKey = 0;

    /**
     * The starting index in the index buffer for this draw call.
     */
    UInt32 firstIndex = 0;

    /**
     * The number of indices to draw.
     */
    UInt32 indexCount = 0;

    /**
     * The polygon fill type to use for this draw command (e.g., solid, wireframe, point).
     */
    polygonFillType::Type polygonFillType = polygonFillType::Undefined;

    /**
     * Graphics API-specific rendering data.
     */
    Variant<OpenGlDrawData> apiDrawData;

    /**
     * Initializes the draw command with rendering data and generates
     * the sort key for proper rendering order.
     *
     * @param modelMatrix The model-to-world transformation matrix.
     * @param material The material to use for rendering.
     * @param distanceToCamera Distance from camera for depth sorting.
     * @param firstIndex Starting index in the index buffer.
     * @param indexCount Number of indices to draw.
     * @param polygonFillType The polygon fill type to use for this draw command.
     * @param apiDrawData Graphics API-specific rendering data.
     */
    void initialize(
      const Matrix4& modelMatrix,
      SharedPtr<IMaterial> material,
      float distanceToCamera,
      UInt32 firstIndex,
      UInt32 indexCount,
      polygonFillType::Type polygonFillType,
      const Variant<OpenGlDrawData>& apiDrawData
    );

    /**
     * Resets the draw command to its default state, clearing all data and
     * setting the model matrix to identity.
     */
    void reset();

    /**
     * Generates a sort key for this draw command based on its render mode,
     * material ID, and distance to the camera.
     *
     * @param renderMode The material render mode (e.g., opaque, transparent).
     * @param materialId The unique identifier of the material used for this draw command.
     * @param distanceToCamera The distance from the camera to the geometry being drawn.
     *
     * @return A 64-bit unsigned integer representing the sort key for this draw command.
     */
    static UInt64 GenerateSortKey(
      materialRenderMode::Type renderMode,
      UInt16 materialId,
      float distanceToCamera
    );
  };
}
