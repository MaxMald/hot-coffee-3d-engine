layout(std140, binding = 9) uniform CubeMapGeneratorBlock
{
    vec4 ucmgInvert;  // Invert components of the cubemap's local position (x, y, z, w) as needed (1.0 for normal, -1.0 for inverted)
};