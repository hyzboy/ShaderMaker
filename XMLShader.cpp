#include"XMLShader.h"
#include"GLSLCompiler.h"

namespace shader_lib
{
    using namespace hgl;

    XMLShader::~XMLShader()
    {
        if(spv_data)
            glsl_compiler::Free(spv_data);
    } 

    bool XMLShader::SetShaderSource(const UTF8StringList &shader_text)
    {
        shader_source=ToString(shader_text,UTF8String(U8_TEXT("\n"),1));

        spv_data=glsl_compiler::CompileShaderToSPV((uint8 *)shader_source.c_str(),shader_type);

        if(!spv_data->result)
        {
            std::cerr<<"GLSL Compiler Error:"<<std::endl;
            std::cerr<<"          log: "<<spv_data->log<<std::endl;
            std::cerr<<"    debug log: "<<spv_data->debug_log<<std::endl;
            std::cerr<<std::endl;

            return(false);
        }

        return(true);
    }

    bool XMLShader::SaveToGLSL(const OSString &filename)
    {
        return filesystem::SaveMemoryToFile(filename,shader_source.c_str(),shader_source.Length());
    } 

    bool XMLShader::SaveToSPV(const OSString &filename)
    {
        if(!spv_data)return(false);

        return filesystem::SaveMemoryToFile(filename,spv_data->spv_data,spv_data->spv_length);
    }

    bool XMLShader::SaveToShader(const OSString &filename)
    {    
        if(!spv_data)return(false);

        return glsl_compiler::SaveSPV2Shader(filename,spv_data,shader_type);
    }
}//namespace shader_lib
