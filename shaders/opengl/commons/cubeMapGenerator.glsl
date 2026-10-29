layout(std140, binding = 9) uniform CubeMapGeneratorBlock
{
    vec4 ucmgInvert;  // Invert components of the cubemap's local position (x, y, z, w) as needed (1.0 for normal, -1.0 for inverted). (used by Equirectangular to Cubemap conversion)
    
    float ucmgSampleDelta;      // The sample delta for generating the diffuse irradiance cube map. Smaller values result in higher quality but take longer to compute. (used by Diffuse Irradiance Cubemap generation)
    float ucmgSourceMipLevel;   // The mip level of the source cubemap to sample from. (used by Diffuse Irradiance Cubemap generation)
    uint ucmgSampleCount;      // The number of samples to use when generating the cubemap. (used by Prefiltered Environment Cubemap generation)
    float ucmgRoughness;        // The roughness value for the prefiltered environment cubemap. (used by Prefiltered Environment Cubemap generation)
};