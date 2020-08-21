#ifndef SHADER_COMPILER_MODULE_INCLUDE
#define SHADER_COMPILER_MODULE_INCLUDE

#include<hgl/type/StringList.h>
#include<hgl/type/DateTime.h>
namespace shader
{
    using namespace hgl;

    struct PushConstant
    {
        UTF8StringList codes;
    };//struct PushConstant

    struct UBO
    {
        UTF8String name;                ///<UBO结构名
        UTF8String value_name;          ///<UBO名变量(会增加vert/frag/geo等前缀)
        UTF8String cpp_file;            ///<c++代码相关文件

        UTF8StringList codes;           ///<代码
    };//struct UBO

    /**
     * 模组
     */
    struct Module
    {
        UTF8String  module_name;        ///<模组名称
        OSString    filename;           ///<模组源文件

        UTF8String  author;             ///<作者名称
        UTF8String  email;              ///<作者email地址

        DateTime    time_first;         ///<初次创作时间
        DateTime    time_last;          ///<最后更新时间

        ObjectList<UBO> ubo_list;
    };//struct Module

    Module *LoadXMLShader(const OSString &filename);
}//namespace shader
#endif//SHADER_COMPILER_MODULE_INCLUDE
