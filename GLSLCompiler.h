#ifndef HGL_GLSL_COMPILER_INCLUDE
#define HGL_GLSL_COMPILER_INCLUDE

#include<hgl/type/BaseString.h>
namespace glsl_compiler
{
    bool Init();
    void Close();
    bool CompileShader(const hgl::OSString &filename);
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
