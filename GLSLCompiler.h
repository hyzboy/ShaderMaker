#ifndef HGL_GLSL_COMPILER_INCLUDE
#define HGL_GLSL_COMPILER_INCLUDE

#include<hgl/type/String.h>
#include<stdint.h>

namespace hgl
{
    namespace io
    {
        class MemoryOutputStream;
    }
}

namespace glsl_compiler
{
    using namespace hgl;

    enum class Descriptor         //等同VkDescriptorType
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER_DYNAMIC,
        STORAGE_BUFFER_DYNAMIC,
        INPUT_ATTACHMENT,

        ENUM_CLASS_RANGE(SAMPLER,INPUT_ATTACHMENT)
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
        ShaderResourceData resource[size_t(Descriptor::RANGE_SIZE)];
    };

    bool Init();
    void Close();

    uint32_t GetType(const char *ext_name);
    
    SPVData *   Compile (const uint32_t type,const char *source);
    void        Free    (SPVData *spv_data);

    SPVData *CompileShaderToSPV(const uint8 *source,const uint32_t flag);    
    bool SaveSPV2Shader(hgl::io::MemoryOutputStream *mos,const SPVData *spv,const uint32_t flag,const bool include_file_header);
    bool SaveSPV2Shader(const OSString &filename,const SPVData *spv,const uint32_t flag);

    void Free(SPVData *spv_data);

    bool CompileShader(const hgl::OSString &filename);
    
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
