/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin [in] ----------------------------------------------------------------
layout(location=0) in vec4 Position;
layout(location=1) in vec4 TexCoord;
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
layout(location=0) out vec4 out_TexCoord;
//-- End [out] -----------------------------------------------------------------

void main()
{
vec4 lt=vec4(Position.xy,vec2(0,1));
vec4 rb=vec4(Position.zw,vec2(0,1));
vec4 lt_fin=world.ortho*lt;
vec4 rb_fin=world.ortho*rb;
out_TexCoord=TexCoord;
gl_Position=vec4(lt_fin.xy,rb_fin.xy);
}
