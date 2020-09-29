/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec3 Color;
//-- End [in] ------------------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
FragColor=vec4(Color,1.0);
}
