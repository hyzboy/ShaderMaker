#include<hgl/type/String.h>

namespace shader_lib
{
    constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");

    bool LoadFromFolder(const hgl::OSString &);
    void Clear();
}//namespace shader_lib
