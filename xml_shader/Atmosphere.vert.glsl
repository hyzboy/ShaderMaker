/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec3 Position;
//-- End [in] ------------------------------------------------------------------

//-- Begin 1 uniforms ----------------------------------------------------------
layout(binding=0,row_major) uniform WorldMatrix    // C++: hgl/graph/WorldMatrix.h
{
    mat4 ortho;
    mat4 projection;
    mat4 inverse_projection;
    mat4 modelview;
    mat4 inverse_modelview;
    mat3 normal;
    layout(offset=384) mat4 mvp;
    mat4 inverse_mvp;
    vec4 view_pos;
    vec2 canvas_resolution;
    vec2 viewport_resolution;
}world;
//-- End uniforms --------------------------------------------------------------

//-- Raw Begin [PushConstant3D] ------------------------------------------------
// max 128 bytes
layout(push_constant) uniform Consts
{
    mat4 local_to_world;
} pc;
//-- Raw End [PushConstant3D] --------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 out_Position;
//-- End [out] -----------------------------------------------------------------

void main()
{
out_Position=world.mvp*pc.local_to_world*vec4(Position,1.0);
gl_Position=out_Position;
}
