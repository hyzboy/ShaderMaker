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

        bool GLSL2SPV(const VkShaderStageFlagBits shader_type,const char *shader_source,std::vector<uint32> &spirv,UTF8String &log,UTF8String &debug_log);
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GLSL2SPV_INCLUDE
