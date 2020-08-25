#include<iostream>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/graph/VertexAttrib.h>
#include"GLSLCompiler.h"

using namespace hgl;
using namespace hgl::io;

constexpr char SHADER_FILE_HEADER[]="Shader\x1A";
constexpr uint SHADER_FILE_HEADER_BYTES=sizeof(SHADER_FILE_HEADER)-1;

void OutputShaderStage(const glsl_compiler::ShaderStageData &ssd,DataOutputStream *dos,const char *hint)
{
    dos->WriteUint8(uint8(ssd.count));

    if(ssd.count<=0)return;

    MemoryOutputStream mos;
    AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

    const glsl_compiler::ShaderStage *ss=ssd.items;
    VertexAttribType vat;
    const char *type_string;

    for(size_t i=0;i<ssd.count;i++)
    {
        mdos->WriteUint8(ss->location);
        mdos->WriteUint8(ss->basetype);
        mdos->WriteUint8(ss->vec_size);
        mdos->WriteAnsiTinyString(ss->name);

        vat.basetype=VertexAttribBaseType(ss->basetype);
        vat.vec_size=ss->vec_size;

        type_string=GetVertexAttribName(&vat);

        std::cout<<"layout(location="<<int(ss->location)<<") "<<hint<<" "<<type_string<<" "<<ss->name<<std::endl;
        ++ss;
    }

    dos->WriteUint32(mos.Tell());
    dos->Write(mos.GetData(),mos.Tell());
}

void OutputShaderResource(const glsl_compiler::ShaderResourceData *srd_arrays,const uint32_t type,DataOutputStream *dos,const char *hint)
{
    const glsl_compiler::ShaderResourceData *srd=srd_arrays+type;

    if(srd->count<=0)return;

    dos->WriteUint32((uint32_t)type);

    MemoryOutputStream mos;
    AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

    std::cout<<srd->count<<" "<<hint<<std::endl;

    mdos->WriteUint8(uint8(srd->count));

    const glsl_compiler::ShaderResource *sr=srd->items;

    for(size_t i=0;i<srd->count;i++)
    {
        mdos->WriteUint8(sr->set);          //append in version 1
        mdos->WriteUint8(sr->binding);
        mdos->WriteAnsiTinyString(sr->name);

        std::cout<<"layout(set="<<int(sr->set)<<",binding="<<int(sr->binding)<<") uniform "<<sr->name<<std::endl;
        ++sr;
    }

    dos->WriteUint32(mos.Tell());
    dos->Write(mos.GetData(),mos.Tell());
}

glsl_compiler::SPVData *CompileShaderToSPV(const uint8 *source,const uint32_t flag)
{
    if(source[0]==0xEF
     &&source[1]==0xBB
     &&source[2]==0xBF)
        source+=3;

    glsl_compiler::SPVData *spv=glsl_compiler::Compile(flag,(char *)source);

    if(!spv)return(false);

    const bool result=spv->result;

    if(!result)
    {
        std::cerr<<"glsl compile failed."<<std::endl;
        std::cerr<<"info: "<<spv->log<<std::endl;
        std::cerr<<"debug info: "<<spv->debug_log<<std::endl;

        glsl_compiler::Free(spv);
        return(nullptr);
    }

    std::cout<<"Compile successed! spv length "<<spv->spv_length<<" bytes."<<std::endl;

    return spv;
}

bool CompileShader(const OSString &filename)
{
    int64 size;

    AutoDeleteArray<uint8> source=(uint8 *)filesystem::LoadFileToMemory(filename,size,true);

    if(!source)
    {
        os_err<<OS_TEXT("Load Shader file [")<<filename.c_str()<<OS_TEXT("] failed!")<<std::endl;
        return(false);
    }
    else
    {
        os_out<<OS_TEXT("Load Shader file [")<<filename.c_str()<<OS_TEXT("] successed!")<<std::endl;
    }

    const UTF8String ext_name=to_u8(filesystem::ClipFileExtName(filename,false));

    const uint32_t flag=glsl_compiler::GetType(ext_name.c_str());

    glsl_compiler::SPVData *spv=CompileShaderToSPV(source,flag);

    if(!spv)
        return(false);

    {
        const OSString spv_filename=filename+OS_TEXT(".spv");

        if(filesystem::SaveMemoryToFile(spv_filename,spv->spv_data,spv->spv_length)!=spv->spv_length)
        {
            os_err<<OS_TEXT("Save SPV file [")<<spv_filename.c_str()<<OS_TEXT("] failed!")<<std::endl;
            std::cerr<<"save to file error!"<<std::endl;
            return(false);
        }
        else
        {
            os_out<<OS_TEXT("Save SPV file [")<<spv_filename.c_str()<<OS_TEXT("] successed!")<<std::endl;
        }
    }

    {
        const OSString sr_filename=filename+OS_TEXT(".shader");

        MemoryOutputStream mos;
        AutoDelete<DataOutputStream> dos=new LEDataOutputStream(&mos);

        dos->Write(SHADER_FILE_HEADER,SHADER_FILE_HEADER_BYTES);
        dos->WriteUint8(1);     //version
        dos->WriteUint32(flag);
        dos->WriteUint32(spv->spv_length);
        dos->Write(spv->spv_data,spv->spv_length);

        OutputShaderStage(spv->input,dos,"in");
        OutputShaderStage(spv->output,dos,"out");
        
        OutputShaderResource(spv->resource,(uint32_t)glsl_compiler::VkDescriptor::COMBINED_IMAGE_SAMPLER,dos,"combined_image_sampler");
        OutputShaderResource(spv->resource,(uint32_t)glsl_compiler::VkDescriptor::UNIFORM_BUFFER,dos,"uniform_buffer");
        OutputShaderResource(spv->resource,(uint32_t)glsl_compiler::VkDescriptor::STORAGE_BUFFER,dos,"storage_buffer");

        if(filesystem::SaveMemoryToFile(sr_filename,mos.GetData(),mos.Tell()))
            os_out<<OS_TEXT("Save Shader file [")<<sr_filename.c_str()<<OS_TEXT("] successed, total ")<<mos.Tell()<<OS_TEXT(" bytes.")<<std::endl;
        else
            os_err<<OS_TEXT("Save Shader file [")<<sr_filename.c_str()<<OS_TEXT("] failed!")<<std::endl;
    }

    return(true);
}

int os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("ShaderCompiler 1.1")<<std::endl;
    os_out<<OS_TEXT("Copyright (C) " HGL_OFFICAL_WEB)<<std::endl;
    os_out<<std::endl;

    if(argc<2)
    {
        os_out<<OS_TEXT("Example: ShaderCompiler [shader filename]")<<std::endl;
        return(0);
    }

    if(!glsl_compiler::Init())
    {
        std::cerr<<"Init GLSLCompiler plug-in failed."<<std::endl;
        return(-1);
    }

    CompileShader(argv[1]);

    glsl_compiler::Close();

    return 0;
}
