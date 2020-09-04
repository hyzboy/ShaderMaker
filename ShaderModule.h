#ifndef SHADER_COMPILER_MODULE_INCLUDE
#define SHADER_COMPILER_MODULE_INCLUDE

#include<hgl/type/StringList.h>
#include<hgl/type/DateTime.h>
#include<hgl/graph/VertexAttrib.h>
namespace shader_lib
{
    using namespace hgl;

    /**
     * XML着色模块类型
     */
    enum class XMLShaderModuleType
    {
        Common=0,           ///<一般公共模块

        PushConstant,

        VertexAttrib,       ///<顶点属性
        FragmentOutput,     ///<片段着色器输出流        

        Vertex,             ///<顶点着色器
        TessControl,
        TessEvaluation,
        Geometry,
        Fragment,
        Compute,

        Raygen,
        AnyHit,
        ClosestHit,
        Miss,
        Intersection,
        Callable,
        Task,
        Mesh,

        ENUM_CLASS_RANGE(Common,Mesh)
    };//enum class XMLShaderModuleType

    struct PushConstant
    {
        UTF8StringList codes;
    };//struct PushConstant

    struct SubModule
    {
        UTF8StringList cpp_files;       ///<c++代码相关文件
    };

    struct UBO:public SubModule
    {
        UTF8String name;                ///<UBO结构名
        UTF8String value_name;          ///<UBO名变量(会增加vert/frag/geo等前缀)

        UTF8StringList codes;           ///<代码
    };//struct UBO

    struct ShaderStage:public SubModule
    {
        graph::VertexAttribType type;
        UTF8String name;
        UTF8String comment;
    };

    /**
     * 模组
     */
    struct Module
    {
        UTF8String  module_name;                ///<模组名称
        OSString    filename;                   ///<模组源文件

        ObjectList<ShaderStage> shader_stage;
        ObjectList<UBO>         ubo_list;
    };//struct Module
}//namespace shader_lib
#endif//SHADER_COMPILER_MODULE_INCLUDE
