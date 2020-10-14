#include"XMLShader.h"
#include"GLSLCompiler.h"
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    using namespace hgl;

    void XMLShader::SetFilename(const OSString &filename)
    {        
        origin_filename=filename;
        short_name=to_u8(filesystem::TrimFileExtName(filename,true));
        ext_name=filesystem::ClipFileExtName(short_name,false);
    }

    XMLShader::~XMLShader()
    {
        if(spv_data)
            glsl_compiler::Free(spv_data);
    } 

    bool XMLShader::SetShaderSource(const UTF8StringList &shader_text,InfoOutput *info_output)
    {
        shader_source=ToString(shader_text,UTF8String(U8_TEXT("\n"),1));

        spv_data=glsl_compiler::CompileShaderToSPV((uint8 *)shader_source.c_str(),shader_type);

        if(!spv_data
         ||!spv_data->result)
        {
            info_output->colorWrite("red","<p>GLSL Compiler Error:");

            if(spv_data)
            {
                info_output->colorWriteln("red","          log: "+UTF8String(spv_data->log));
                info_output->colorWriteln("red","    debug log: "+UTF8String(spv_data->debug_log));
                info_output->write("</p>");
            }

            return(false);
        }

        info_output->colorWriteln("green","<p>GLSL Compiler OK.</p>");

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
