/**
 * the Shader created by ShaderMaker (www.hyzgame.com)
 */
#version 450 core

//-- Begin 1 uniforms ----------------------------------------------------------
layout(binding=64,row_major) uniform WorldMatrix    // C++: hgl/graph/WorldMatrix.h
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
}frag_world;
//-- End uniforms --------------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
FragColor=vec4(gl_FragCoord.xy/frag_world.viewport_resolution,0,1);
}
