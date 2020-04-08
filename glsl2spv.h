#ifndef HGL_GRAPH_GLSL2SPV_INCLUDE
#define HGL_GRAPH_GLSL2SPV_INCLUDE

#include<vulkan/vulkan.h>
#include<vector>
#include<hgl/type/BaseString.h>

namespace hgl
{
    namespace graph
    {
        bool InitShaderMaker();
        void ClearShaderMaker();

        bool CompileShader(const OSString& filename);
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GLSL2SPV_INCLUDE
