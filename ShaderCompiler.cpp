#include"glsl2spv.h"
#include<glslang/Public/ShaderLang.h>
#include<iostream>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include"VKShaderParse.h"

using namespace hgl;
using namespace hgl::io;
using namespace hgl::graph;

VK_NAMESPACE_USING

constexpr char SHADER_FILE_HEADER[]="Shader\x1A";
constexpr uint SHADER_FILE_HEADER_BYTES=sizeof(SHADER_FILE_HEADER)-1;

constexpr char *SPIRTypeBaseTypeName[]=
{
    "Unknown",
    "Void",
    "Boolean",
    "SByte",
    "UByte",
    "Short",
    "UShort",
    "Int",
    "UInt",
    "Int64",
    "UInt64",
    "AtomicCounter",
    "Half",
    "Float",
    "Double",
    "Struct",
    "Image",
    "SampledImage",
    "Sampler",
    "AccelerationStructureNV",

    // Keep internal types at the end.
    "ControlPointArray",
    "Char"
};

void OutputShaderStage(ShaderParse *sp,const SPVResVector &stages,DataOutputStream *dos,const char *hint)
{
    uint attr_count=stages.size();

    dos->WriteUint8(attr_count);

    if(attr_count<=0)return;

    MemoryOutputStream mos;
    AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

    spirv_cross::SPIRType::BaseType base_type;
    uint8 vec_size;
    uint location;
    AnsiString name;

    os_out<<hint<<" State: "<<attr_count<<std::endl;
    
    for(const spirv_cross::Resource &si:stages)
    {
        sp->GetFormat(si,&base_type,&vec_size);
        name=sp->GetName(si);
        location=sp->GetLocation(si);

        mdos->WriteUint8(location);
        mdos->WriteUint8(base_type);
        mdos->WriteUint8(vec_size);
        mdos->WriteAnsiTinyString(name);

        std::cout<<hint<<" State ["<<name.c_str()<<"] location="<<location<<", basetype:"<<SPIRTypeBaseTypeName[base_type-spirv_cross::SPIRType::BaseType::Unknown]<<", vecsize: "<<uint(vec_size)<<std::endl;
    }

    dos->WriteUint32(mos.GetSize());
    dos->Write(mos.GetData(),mos.GetSize());
}

void OutputShaderResource(ShaderParse *sp,DataOutputStream *dos,const SPVResVector &res,const enum VkDescriptorType desc_type,const char *hint)
{
    uint32_t count=res.size();

    if(count<=0)return;

    dos->WriteUint32(desc_type);

    MemoryOutputStream mos;
    AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

    std::cout<<count<<" "<<hint<<std::endl;

    mdos->WriteUint8(count);

    uint binding;
    AnsiString name;

    for(const spirv_cross::Resource &obj:res)
    {
        name    =sp->GetName(obj);
        binding =sp->GetBinding(obj);

        mdos->WriteUint8(binding);
        mdos->WriteAnsiTinyString(name);

        std::cout<<hint<<"["<<name.c_str()<<"] binding: "<<binding<<std::endl;
    }

    dos->WriteUint32(mos.GetSize());
    dos->Write(mos.GetData(),mos.GetSize());
}

void OutputShaderConfig(ShaderParse *sp,DataOutputStream *dos)
{
    OutputShaderResource(sp,dos,sp->GetUBO(),       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          "uniform_buffer");
    OutputShaderResource(sp,dos,sp->GetSampler(),   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  "combined_image_sampler");
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
    uint32 spv_size=0;

    if(CompileShaderToSPV(source,ext_name,spirv,flag))
    {
        const OSString spv_filename=filename+OS_TEXT(".spv");
        spv_size=spirv.size()*sizeof(uint32);

        if(filesystem::SaveMemoryToFile(spv_filename,spirv.data(),spv_size)!=spv_size)
        {
            std::cerr<<"save to file error!"<<std::endl;
            return(false);
        }
    }

    {
        const OSString sr_filename=filename+OS_TEXT(".shader");

        OpenFileOutputStream fos(sr_filename,fomCreateTrunc);
        DataOutputStream *dos=new LEDataOutputStream(fos);

        dos->Write(SHADER_FILE_HEADER,SHADER_FILE_HEADER_BYTES);
        dos->WriteUint8(0);
        dos->WriteUint32(flag);
        dos->WriteUint32(spv_size);
        dos->Write(spirv.data(),spv_size);

        os_out<<OS_TEXT("SPV Data: ")<<spv_size<<OS_TEXT(" bytes.")<<std::endl;

        ShaderParse sp(spirv.data(),spirv.size()*sizeof(uint32));

        OutputShaderStage(&sp,sp.GetStageInputs(),dos,"Input");
        OutputShaderStage(&sp,sp.GetStageOutputs(),dos,"Output");
        OutputShaderConfig(&sp,dos);

        delete dos;
        fos->Close();
    }

    return(true);
}

int os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("ShaderCompiler 1.0")<<std::endl;
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
