#ifndef HGL_GLSL_COMPILER_INCLUDE
#define HGL_GLSL_COMPILER_INCLUDE

#include<inttypes.h>
#include<hgl/graph/VertexAttrib.h>

using namespace hgl;
using namespace hgl::graph;

namespace glsl_compiler
{
    enum class VkDescriptor         //等同VkDescriptorType
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER = 1,
        SAMPLED_IMAGE = 2,
        STORAGE_IMAGE = 3,
        UNIFORM_TEXEL_BUFFER = 4,
        STORAGE_TEXEL_BUFFER = 5,
        UNIFORM_BUFFER = 6,
        STORAGE_BUFFER = 7,
        UNIFORM_BUFFER_DYNAMIC = 8,
        STORAGE_BUFFER_DYNAMIC = 9,
        INPUT_ATTACHMENT = 10,

        BEGIN_RANGE=SAMPLER,
        END_RANGE=INPUT_ATTACHMENT,
        RANGE_SIZE=(END_RANGE-BEGIN_RANGE)+1
    };

    struct ShaderStage
    {
        char name[128];
        uint8_t location;
        uint32_t basetype;      //现在改对应hgl/graph/VertexAttrib中的enum class VertexAttribBaseType
        uint32_t vec_size;
    };//

    struct ShaderStageData
    {
        uint32_t count;
        ShaderStage *items;
    };

    struct ShaderResource
    {
        char name[128];

        uint8_t set;
        uint8_t binding;
    };

    struct ShaderResourceData
    {
        uint32_t count;
        ShaderResource *items;
    };

    struct SPVData
    {
        bool result;
        char *log;
        char *debug_log;

        uint32_t *spv_data;
        uint32_t spv_length;

        ShaderStageData input,output;
        ShaderResourceData resource[size_t(VkDescriptor::RANGE_SIZE)];
    };

    bool        Init    ();
    void        Close   ();

    uint32_t    GetType (const char *ext_name);
    SPVData *   Compile (const uint32_t type,const char *source);

    void        Free    (SPVData *);
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
