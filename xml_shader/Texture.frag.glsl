/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec2 TexCoord;
//-- End [in] ------------------------------------------------------------------

//-- Begin 1 uniforms ----------------------------------------------------------
layout(binding=64) uniform sampler2D tex;
//-- End uniforms --------------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
FragColor=texture(tex,TexCoord);
}
