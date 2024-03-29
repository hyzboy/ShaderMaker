﻿#pragma once

#include<hgl/type/StringList.h>
#include<hgl/type/SortedSets.h>
#include"InfoOutput/InfoOutput.h"
#include"vulkan/VKShaderCommon.h"

namespace glsl_compiler
{
    struct SPVData;
};

namespace shader_lib
{
    using namespace hgl;
    using namespace vk_shader;

    enum class DescriptorSetsType
    {
        //设计使其对应shader中的sets
    
        Global=0,   ///<全局参数(如太阳光等)
        Material,   ///<材质中永远不变的参数
    //    Texture,    ///<材质中的纹理参数
        Value,      ///<材质中的变量参数
        Renderable, ///<渲染实例参数(如Local2World matrix)

        ENUM_CLASS_RANGE(Global,Renderable)
    };//

    inline const DescriptorSetsType DescriptorSetsTypeFromName(const char *name)
    {
        if(!name||!*name||name[1]!='_')return DescriptorSetsType::Value;

        if(*name=='m')return DescriptorSetsType::Material;
        if(*name=='g')return DescriptorSetsType::Global;
        if(*name=='r')return DescriptorSetsType::Renderable;
                      return DescriptorSetsType::Value;
    }

    struct Uniform
    {
        UTF8String type_name;
        UTF8String value_name;

        DescriptorSetsType type;

        //以下变量由material统计生成
        int set_number=-1;
        int binding=-1;

    public:

        Uniform()
        {
            type=DescriptorSetsType::Value;
        }

        Uniform(const UTF8String &tn,const UTF8String &vn)
        {
            type_name=tn;
            value_name=vn;

            type=DescriptorSetsTypeFromName(value_name.c_str());
        }

        Uniform(Uniform *u)
        {
            type=u->type;
            value_name=u->value_name;

            type=u->type;
        }
    };

    struct GeometryAttribute
    {
        UTF8String in;
        UTF8String out;
        uint max_vertices;
    };

    struct XMLShaderModule
    {
        UTF8StringList depend_raw_list;
        UTF8StringList struct_list;
        UTF8StringList codes;
    };//

    struct XMLShader
    {
        OSString origin_filename;
        UTF8String short_name;
        UTF8String ext_name;

        UTF8StringList in;
        UTF8StringList out;
        UTF8StringList raw;
        UTF8StringList struct_block;
        UTF8StringList modules;
        ObjectList<Uniform> uniforms;

        GeometryAttribute geom;

        UTF8StringList main;

    public:

        uint32_t shader_type;
        UTF8String shader_source;
        glsl_compiler::SPVData *spv_data;

    public:

        XMLShader()
        {
            shader_type=0;
            spv_data=nullptr;
        }

        ~XMLShader();

        void SetFilename(const OSString &);

        bool SetShaderSource(const UTF8StringList &,InfoOutput *);
    };//struct XMLShader
}//namespace shader_lib
