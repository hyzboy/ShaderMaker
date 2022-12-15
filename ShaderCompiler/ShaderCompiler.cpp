#include<iostream>
#include"GLSLCompiler/GLSLCompiler.h"

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

    glsl_compiler::CompileShader(argv[1]);

    glsl_compiler::Close();

    return 0;
}
