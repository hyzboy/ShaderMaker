/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec2 TexCoord;
//-- End [in] ------------------------------------------------------------------

//-- Begin 2 uniforms ----------------------------------------------------------
layout(binding=64) uniform sampler2D lum_texture;
layout(binding=65,row_major) uniform ColorMaterial
{
    vec4 color;
}color_material;
//-- End uniforms --------------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
float lum=texture(lum_texture,TexCoord).r;
FragColor=vec4(color_material.color.rgb*lum,color_material.color.a);
}
