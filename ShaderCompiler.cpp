#include"glsl2spv.h"
#include<glslang/Public/ShaderLang.h>
#include<iostream>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include"VKShaderParse.h"

using namespace hgl;
using namespace hgl::io;
using namespace hgl::graph;

VK_NAMESPACE_USING

/**
 *  shader file header
 *
 *  offset  size    
 *  0       7       flag string "Shader\x1A"
 *  7       1       version, now is 0
 *  8       4       VkShaderStageFlagBits              
 *  [vertex input attribute]
  */
constexpr char SHADER_FILE_HEADER[]="Shader\x1A";
constexpr uint SHADER_FILE_HEADER_BYTES=sizeof(SHADER_FILE_HEADER)-1;

void OutputVertexShaderConfig(ShaderParse *sp,DataOutputStream *dos)
{
    Map<UTF8String,VkVertexInputAttributeDescription *> stage_input_locations;

    const SPVResVector &stage_inputs=sp->GetStageInputs();

    uint attr_count=stage_inputs.size();

    dos->WriteUint8(attr_count);
    spirv_cross::SPIRType::BaseType base_type;
    uint8 vec_size;
    
    for(const spirv_cross::Resource &si:stage_inputs)
    {
        sp->GetFormat(si,&base_type,&vec_size);

        dos->WriteUint8(sp->GetLocation(si));
        dos->WriteUint8(base_type);
        dos->WriteUint8(vec_size);
        dos->WriteUTF8TinyString(sp->GetName(si));
    }
}

void OutputShaderResource(ShaderParse *sp,DataOutputStream *dos,const SPVResVector &res,const enum VkDescriptorType desc_type)
{
    uint32_t count=res.size();

    dos->WriteUint32(desc_type);
    dos->WriteUint8(count);

    if(count<=0)return;

    uint binding;
    UTF8String name;

    for(const spirv_cross::Resource &obj:res)
    {
        name    =sp->GetName(obj);
        binding =sp->GetBinding(obj);

        dos->WriteUint8(binding);
        dos->WriteUTF8TinyString(name);
    }
}

void OutputShaderConfig(ShaderParse *sp,DataOutputStream *dos)
{
    OutputShaderResource(sp,dos,sp->GetUBO(),       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    OutputShaderResource(sp,dos,sp->GetSampler(),   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
}

bool CompileShader(const OSString &filename)
{
    int64 size;

    AutoDeleteArray<char> source=(char *)filesystem::LoadFileToMemory(filename,size,true);

    if(!source)
        return(false);

    const OSString ext_name=filesystem::ClipFileExtName(filename,false);

    VkShaderStageFlagBits flag;
    SPIRVData spirv;

    if(CompileShaderToSPV(source,ext_name,spirv,flag))
    {
        const OSString spv_filename=filename+OS_TEXT(".spv");
        const uint64 spv_size=spirv.size()*sizeof(uint32);

        if(filesystem::SaveMemoryToFile(spv_filename,spirv.data(),spv_size)!=spv_size)
        {
            std::cerr<<"save to file error!"<<std::endl;
            return(false);
        }
    }

    {
        const OSString sr_filename=filename+OS_TEXT(".sr");

        OpenFileOutputStream fos(sr_filename,fomCreateTrunc);
        DataOutputStream *dos=new LEDataOutputStream(fos);

        dos->Write(SHADER_FILE_HEADER,SHADER_FILE_HEADER_BYTES);
        dos->WriteUint8(0);
        dos->WriteUint32(flag);

        ShaderParse sp(spirv.data(),spirv.size()*sizeof(uint32));

        if (ext_name.CaseComp(OS_TEXT("vert")) == 0)
            OutputVertexShaderConfig(&sp,dos);
    
        OutputShaderConfig(&sp,dos);

        delete dos;
        fos->Close();
    }

    return(true);
}

int os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("ShaderCompiler 1.0 (glslang version:")<<GLSLANG_MINOR_VERSION<<OS_TEXT(")")<<std::endl;
    os_out<<OS_TEXT("Copyright (C) www.hyzgame.com")<<std::endl;
    os_out<<std::endl;

    if(argc<2)
    {
        os_out<<OS_TEXT("Example: ShaderCompiler [shader filename]")<<std::endl;
        return(0);
    }

    InitShaderCompiler();

    CompileShader(argv[1]);

    CloseShaderCompiler();

    return 0;
}
