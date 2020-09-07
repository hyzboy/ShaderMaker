#include<hgl/type/String.h>

namespace shader_lib
{
    using namespace hgl;

    constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");

    bool LoadFromFolder(const OSString &);
    void Clear();

    bool LoadXMLShader(const OSString &);
}//namespace shader_lib
