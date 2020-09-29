/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin 1 uniforms ----------------------------------------------------------
layout(binding=64,row_major) uniform ColorMaterial
{
    vec4 color;
}color_material;
//-- End uniforms --------------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
FragColor=color_material.color;
}
