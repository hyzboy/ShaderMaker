#ifndef HGL_GRAPH_GLSL2SPV_INCLUDE
#define HGL_GRAPH_GLSL2SPV_INCLUDE

#include<vulkan/vulkan.h>
#include<vector>
#include<hgl/type/BaseString.h>

namespace hgl
{
    namespace graph
    {    
        using SPIRVData=std::vector<uint32>;

        bool InitShaderCompiler();
        void CloseShaderCompiler();

        /**
         * 编译一段shader到spv
         * @param source 原始的GLSL代码
         * @param ext_name glsl类型
         * @param spirv 输出的spir-v数据
         * @return 是否编译成功
         */
        bool CompileShaderToSPV(const char *source,const OSString &ext_name,SPIRVData &spirv);
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GLSL2SPV_INCLUDE
