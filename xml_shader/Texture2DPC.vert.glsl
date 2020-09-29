/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec2 Position;
layout(location=1) in vec2 TexCoord;
//-- End [in] ------------------------------------------------------------------

//-- Raw Begin [PushConstant3D] ------------------------------------------------
// max 128 bytes
layout(push_constant) uniform Consts
{
    mat4 local_to_world;
} pc;
//-- Raw End [PushConstant3D] --------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec2 out_TexCoord;
//-- End [out] -----------------------------------------------------------------

void main()
{
out_TexCoord=TexCoord;
gl_Position=pc.local_to_world*vec4(Position,0.0,1.0);
}
