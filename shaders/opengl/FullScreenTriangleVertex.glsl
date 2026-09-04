#version 420 core

layout(location = 0) out vec2 vTexCoord;

void main()
{
  // Generate a full-screen triangle using vertex ID
  // 3 vertices cover the entire screen
  vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
  );

  vec2 texCoords[3] = vec2[](
    vec2(0.0, 0.0),
    vec2(2.0, 0.0),
    vec2(0.0, 2.0)
  );

  vTexCoord = texCoords[gl_VertexID];
  gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}