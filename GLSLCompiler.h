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

    using ShaderType=uint32_t;

    enum class DescriptorType         //等同VkDescriptorType
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

    constexpr size_t SHADER_STAGE_NAME_MAX_LENGTH=128;

    struct ShaderStage
    {
        char name[SHADER_STAGE_NAME_MAX_LENGTH];
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
        char name[SHADER_STAGE_NAME_MAX_LENGTH];

        uint8_t set;
        uint8_t binding;

    public:

        ShaderResource(ShaderResource *sr)
        {
            memcpy(name,sr->name,SHADER_STAGE_NAME_MAX_LENGTH);
            set=sr->set;
            binding=sr->binding;
        }

        const int Comp(const ShaderResource &sr)const
        {
            if(set!=sr.set)return sr.set-set;
            if(binding!=sr.binding)return sr.binding-binding;

            return strcmp(name,sr.name);
        }

        CompOperator(const ShaderResource &,Comp);
    };//struct ShaderResource

    struct MaterialShaderResource:public ShaderResource
    {
        uint32_t shader_stage_flag;

    public:

        MaterialShaderResource(const ShaderType flag,ShaderResource *sr):ShaderResource(sr)
        {
            shader_stage_flag=flag;
        }

        const int Comp(const MaterialShaderResource &msr)const
        {
            int result=ShaderResource::Comp(msr);

            if(result)return result;

            return shader_stage_flag-msr.shader_stage_flag;
        }

        CompOperator(const MaterialShaderResource &,Comp);
    };//struct MaterialShaderResource:public ShaderResource

    using MSRList=MapObject<AnsiString,MaterialShaderResource>;

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
        ShaderResourceData resource[size_t(DescriptorType::RANGE_SIZE)];
    };

    bool Init();
    void Close();

    ShaderType  GetType (const char *ext_name);
    
    SPVData *   Compile (const uint32_t type,const char *source);
    void        Free    (SPVData *spv_data);

    SPVData *CompileShaderToSPV(const uint8 *source,const uint32_t flag);    
    bool SaveSPV2Shader(hgl::io::MemoryOutputStream *mos,const SPVData *spv,const ShaderType flag,const bool include_file_header);
    bool SaveSPV2Shader(const OSString &filename,const SPVData *spv,const ShaderType flag);

    void Free(SPVData *spv_data);

    bool CompileShader(const hgl::OSString &filename);
    
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
