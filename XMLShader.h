#pragma once

#include<hgl/type/StringList.h>
#include<hgl/type/Sets.h>
#include"InfoOutput.h"

namespace glsl_compiler
{
    struct SPVData;
};

namespace shader_lib
{
    using namespace hgl;

    enum ShaderStageBits        //等同VkShaderStageFlagBits
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

    using ShaderType=uint32_t;      //same define in GLSLCompiler.h

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
        int set_number;
        int binding;

    public:

        Uniform()
        {
            type=DescriptorSetsType::Value;
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

        bool SaveToGLSL(const OSString &filename);
        bool SaveToSPV(const OSString &filename);
        bool SaveToShader(const OSString &filename);
    };//struct XMLShader
}//namespace shader_lib
