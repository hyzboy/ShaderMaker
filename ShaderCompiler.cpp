#include"glsl2spv.h"
#include<iostream>

using namespace hgl;
using namespace hgl::graph;

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