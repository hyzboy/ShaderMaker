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

        glsl_compiler::SPVData *spv=glsl_compiler::Compile(shader_type,shader_source.c_str());

        if(!spv)return(false);

        const bool result=spv->result;

        if(!result)
        {
            if(info_output)
            {
                info_output->colorWrite("red",  "<b>glsl compile failed.</b><ul>"
                                                "<li><b>info: </b>"+UTF8String(spv->log)+
                                                "</li><li><b>debug info: </b>"+UTF8String(spv->debug_log)+
                                                "</li></ul>");
            }

            glsl_compiler::Free(spv);
            return(nullptr);
        }

        if(info_output)
        info_output->colorWrite("green","<p><b>Compile successed! spv length "+UTF8String::valueOf(spv->spv_length)+" bytes.</b></p>");

        spv_data=spv;        
        return true;
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
