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

    bool CheckVarying(const UTF8String &name);
    bool CheckVarying(const UTF8StringList &vary_list);
    
    VaryingConfig *GetVarying(const UTF8String &name);

    struct XMLShader
    {
        UTF8StringList in;
        UTF8StringList out;
        UTF8StringList modules;

        UTF8StringList main;
    };//struct XMLShader

    XMLShader *LoadXMLShader(const OSString &);
}//namespace shader_lib
