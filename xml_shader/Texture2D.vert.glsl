/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec2 Position;
layout(location=1) in vec2 TexCoord;
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

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec2 out_TexCoord;
//-- End [out] -----------------------------------------------------------------

void main()
{
out_TexCoord=TexCoord;
gl_Position=world.ortho*vec4(Position,0.0,1.0);
}
