#include"VKShaderCommon.h"

namespace vk_shader
{
    const char *GetShaderStageName(const ShaderType &type)
    {
        if(type==0
         ||type<ssbCallable)return(nullptr);

        uint32_t index=0;

        while(type>1)
            ++index;

        return shader_stage_name_list[index];
    }
}//namespace vk_shader
