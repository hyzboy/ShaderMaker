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

//-- Raw Begin [HQFilter] ------------------------------------------------------
vec4 hqfilter(sampler2D samp, vec2 tc)
{
    // Get the size of the texture we'll be sampling from
    vec2 texSize = textureSize(samp, 0);

    // Scale our input texture coordinates up, move to center of texel
    vec2 uvScaled = tc * texSize + 0.5;

    // Find integer and fractional parts of texture coordinate
    vec2 uvInt = floor(uvScaled);
    vec2 uvFrac = fract(uvScaled);

    // Replace fractional part of texture coordinate
    uvFrac = smoothstep(0.0, 1.0, uvFrac);

    // Reassemble texture coordinate, remove bias, and
    // scale back to 0.0 - 1.0 ranage
    vec2 uv = (uvInt + uvFrac - 0.5) / texSize;

    // Regular texture lookup
    return texture(samp, uv);
}
//-- Raw End [HQFilter] --------------------------------------------------------

//-- Begin [out] ---------------------------------------------------------------
layout(location=0) out vec4 FragColor;
//-- End [out] -----------------------------------------------------------------

void main()
{
FragColor=hqfilter(tex,TexCoord);
}
