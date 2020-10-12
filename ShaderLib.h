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

    bool CheckVarying(const UTF8StringList &vary_list);
    bool CheckRawModule(const Sets<UTF8String> &raw_list);
    bool CheckStruct(const UTF8String &struct_name);
    bool CheckStruct(const UTF8StringList &struct_list);

    bool CheckModule(const UTF8String &module_name);
    bool CheckModules(const UTF8StringList &module_list);

    XMLShaderModule *GetShaderModule(const UTF8String &module_name);
    
    VaryingConfig *GetVarying(const UTF8String &name);    
    UTF8StringList *GetRawModule(const UTF8String &name);
    bool AddStruct(UTF8StringList &shader_text,const UTF8String &front,const UTF8String &name,const UTF8String &back);

    XMLShader *LoadXMLShader(const OSString &,InfoOutput *);

    enum ShaderStageBits        //等同VkShaderStageFlagBits
    {
        ssbVertex       = 0x00000001,
        ssbTesc         = 0x00000002,
        ssbTesv         = 0x00000004,
        ssbGeometry     = 0x00000008,
        ssbFragment     = 0x00000010,
        ssbCompute      = 0x00000020,
        ssbGraphics     = 0x0000001F,
        ssbAll          = 0x7FFFFFFF,
        ssbRaygen       = 0x00000100,
        ssbAnyHit       = 0x00000200,
        ssbClosestHit   = 0x00000400,
        ssbMiss         = 0x00000800,
        ssbIntersection = 0x00001000,
        ssbCallable     = 0x00002000,
        ssbTask         = 0x00000040,
        ssbMesh         = 0x00000080
    };

    struct XMLMaterial
    {
        uint32 shader_bits=0;
        MapObject<uint32,XMLShader> shaders;
    };//struct XMLMaterial

    XMLShader *LoadXMLShader(const OSString &filename);
    bool XMLShaderMaker(const OSString &filename,XMLShader *xs);
}//namespace shader_lib
