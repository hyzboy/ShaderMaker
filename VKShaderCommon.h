#pragma once

#include<stdint.h>
#include<hgl/TypeFunc.h>
#include<hgl/CompOperator.h>

namespace vk_shader
{
    enum ShaderStageBits        //��ͬVkShaderStageFlagBits
    {
        ssbVertex       = 0x00000001,
        ssbTesc         = 0x00000002,
        ssbTesv         = 0x00000004,
        ssbGeometry     = 0x00000008,
        ssbFragment     = 0x00000010,
        ssbGraphics     = 0x0000001F,
        ssbCompute      = 0x00000020,
        ssbTask         = 0x00000040,
        ssbMesh         = 0x00000080,
        ssbRaygen       = 0x00000100,
        ssbAnyHit       = 0x00000200,
        ssbClosestHit   = 0x00000400,
        ssbMiss         = 0x00000800,
        ssbIntersection = 0x00001000,
        ssbCallable     = 0x00002000,
        ssbAll          = 0x7FFFFFFF
    };

    using ShaderType=uint32_t;          ///<����������ʾSHADER����һ��
    
    constexpr char *shader_stage_name_list[32]=
    {
        "Vertex"      ,
        "Tesc"        ,
        "Tesv"        ,
        "Geometry"    ,
        "Fragment"    ,
        "Compute"     ,
        "Task"        ,
        "Mesh"        ,
        "Raygen"      ,
        "AnyHit"      ,
        "ClosestHit"  ,
        "Miss"        ,
        "Intersection",
        "Callable"
    };//

    const char *GetShaderStageName(const ShaderType &);
    
    enum class DescriptorType         //��ͬVkDescriptorType
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER_DYNAMIC,
        STORAGE_BUFFER_DYNAMIC,
        INPUT_ATTACHMENT,

        ENUM_CLASS_RANGE(SAMPLER,INPUT_ATTACHMENT)
    };
    
    constexpr size_t SHADER_RESOURCE_NAME_MAX_LENGTH=128;

    struct ShaderResource
    {
        char name[SHADER_RESOURCE_NAME_MAX_LENGTH];

        uint8_t set;
        uint8_t binding;

    public:

        ShaderResource(const ShaderResource *sr)
        {
            memcpy(name,sr->name,SHADER_RESOURCE_NAME_MAX_LENGTH);
            set=sr->set;
            binding=sr->binding;
        }

        const int Comp(const ShaderResource &sr)const
        {
            if(set!=sr.set)return sr.set-set;
            if(binding!=sr.binding)return sr.binding-binding;

            return strcmp(name,sr.name);
        }

        CompOperator(const ShaderResource &,Comp);
    };//struct ShaderResource
};//namespace vk_shader