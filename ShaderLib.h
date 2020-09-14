#include<hgl/type/StringList.h>

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

    bool CheckVarying(const UTF8StringList &vary_list);
    bool CheckRawModule(const UTF8StringList &raw_list);
    bool CheckStruct(const UTF8StringList &struct_list);
    
    VaryingConfig *GetVarying(const UTF8String &name);    
    UTF8StringList *GetRawModule(const UTF8String &name);
    bool AddStruct(UTF8StringList &shader_text,const UTF8String &front,const UTF8String &name,const UTF8String &back);

    struct Uniform
    {
        UTF8String type_name;
        UTF8String value_name;
    };

    struct XMLShader
    {
        UTF8StringList in;
        UTF8StringList out;
        UTF8StringList raw;
        UTF8StringList struct_block;
        ObjectList<Uniform> uniforms;

        UTF8StringList main;
    };//struct XMLShader

    XMLShader *LoadXMLShader(const OSString &);
}//namespace shader_lib
