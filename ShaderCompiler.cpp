#include"glsl2spv.h"
#include<glslang/Public/ShaderLang.h>
#include<iostream>
#include<hgl/filesystem/FileSystem.h>

using namespace hgl;
using namespace hgl::graph;

bool CompileShader(const OSString &filename)
{
    int64 size;

    AutoDeleteArray<char> source=(char *)filesystem::LoadFileToMemory(filename,size,true);

    if(!source)
        return(false);

    const OSString ext_name=filesystem::ClipFileExtName(filename,false);

    SPIRVData spirv;

    if(CompileShaderToSPV(source,ext_name,spirv))
    {
        const OSString spv_filename=filename+OS_TEXT(".spv");
        const uint64 spv_size=spirv.size()*sizeof(uint32);

        if(filesystem::SaveMemoryToFile(spv_filename,spirv.data(),spv_size)!=spv_size)
        {
            std::cerr<<"save to file error!"<<std::endl;
            return(false);
        }
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
