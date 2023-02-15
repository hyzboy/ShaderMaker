#pragma once

#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include"ShaderLibrary/XMLShader.h"
#include"InfoOutput/InfoOutput.h"

namespace shader_lib
{
    using namespace hgl;

    constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");
    
    bool LoadFromFolder(const OSString &);
    void Clear();

    struct Varying
    {
        UTF8String type;
        UTF8String name;
        UTF8String interpolation;
        UTF8String comment;
    };//
    
    using VaryingConfig=ObjectList<Varying>;

    const UTF8StringList &GetVaryingList();
    const UTF8StringList &GetRawModuleList();
    const UTF8StringList &GetStructList();
    const UTF8StringList &GetModuleList();

    bool CheckVarying(const UTF8StringList &vary_list,InfoOutput *);
    bool CheckRawModule(const UTF8StringList &raw_list,InfoOutput *);
    bool CheckStruct(const UTF8String &struct_name);
    bool CheckStruct(const UTF8StringList &struct_list,InfoOutput *);

    bool CheckModule(const UTF8String &module_name);
    bool CheckModules(const UTF8StringList &module_list,InfoOutput *);

    XMLShaderModule *GetShaderModule(const UTF8String &module_name);
    
    VaryingConfig *GetVarying(const UTF8String &name);
    UTF8StringList *GetRawModule(const UTF8String &name);
    bool AddStruct(UTF8StringList &shader_text,const UTF8String &front,const UTF8String &name,const UTF8String &back);

    struct MaterialShaderResource:public ShaderResource
    {
        uint32_t shader_stage_flag;
        DescriptorType desc_type;

    public:

        MaterialShaderResource(const ShaderType flag,const DescriptorType type,const ShaderResource *sr):ShaderResource(sr)
        {
            shader_stage_flag=flag;
            desc_type=type;
        }

        const int Comp(const MaterialShaderResource &msr)const
        {
            int result=ShaderResource::Comp(msr);

            if(result)return result;

            result=shader_stage_flag-msr.shader_stage_flag;

            if(result)return result;

            return (int)desc_type-(int)msr.desc_type;
        }

        CompOperator(const MaterialShaderResource &,Comp);
    };//struct MaterialShaderResource:public ShaderResource

    using MSRList=ObjectMap<AnsiString,MaterialShaderResource>;

    struct MaterialUniform:public Uniform
    {
        uint32 shader_stage_flag;

    public:

        MaterialUniform(const uint32 st,Uniform *u):Uniform(u)
        {
            shader_stage_flag=st;
        }
    };

    struct DescSetUniformList
    {
        int set_number=-1;

        List<Uniform *> uniform_list;

        ObjectMap<UTF8String,MaterialUniform> mu_list;

    public:

        void Add(const uint32,Uniform *);

        void RefreshDescSetBinding(const int);
    };

    /**
     * 材质信息统计
     */
    struct MaterialStat
    {
        DescSetUniformList ds_uniform[(size_t)DescriptorSetType::RANGE_SIZE];
    };

    using XMLShaderMap=ObjectMap<ShaderType,XMLShader>;

    struct XMLMaterial
    {
        uint32 shader_stage_bits=0;

        MaterialStat mtl_stat;

        XMLShaderMap shader_map;
    };//struct XMLMaterial
    
    XMLShader *LoadXMLShader(io::InputStream *is,InfoOutput *);
    XMLShader *LoadXMLShader(const OSString &filename,InfoOutput *);

    bool XMLShaderMaker(XMLShader *xs,MaterialStat *,InfoOutput *);

    XMLMaterial *LoadXMLMaterial(const OSString &filename,InfoOutput *info_output);
    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output);
}//namespace shader_lib
