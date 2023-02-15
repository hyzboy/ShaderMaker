#include"ShaderLibrary/XMLShader.h"
#include"GLSLCompiler/GLSLCompiler.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/MergeString.h>

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
        shader_source=MergeString(shader_text,UTF8String(U8_TEXT("\n"),1));

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
            return(false);
        }

        if(info_output)
        info_output->colorWrite("green","<p><b>Compile successed! spv length "+UTF8String::numberOf(spv->spv_length)+" bytes.</b></p>");

        spv_data=spv;        
        return true;
    }
}//namespace shader_lib
