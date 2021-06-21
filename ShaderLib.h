#pragma once

#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include"XMLShader.h"
#include"InfoOutput.h"

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

    /**
     * shader统计信息.
     */
    struct ShaderStat
    {
        int set[(size_t)DescriptorSetsType::RANGE_SIZE];
        int binding_count;

        ShaderStat()
        {
            hgl_zero(set);
            hgl_zero(binding_count);
        }
    };

    using XMLShaderMap=MapObject<uint32,XMLShader>;

    struct XMLMaterial
    {
        uint32 shader_bits=0;

        ShaderStat shader_stat;

        XMLShaderMap shaders;
    };//struct XMLMaterial
    
    XMLShader *LoadXMLShader(io::InputStream *is,InfoOutput *);
    XMLShader *LoadXMLShader(const OSString &filename,InfoOutput *);

    bool XMLShaderMaker(XMLShader *xs,ShaderStat *,InfoOutput *);

    XMLMaterial *LoadXMLMaterial(const OSString &filename,InfoOutput *info_output);
    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output);
}//namespace shader_lib
