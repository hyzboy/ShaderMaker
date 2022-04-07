#include<hgl/type/String.h>
#include<iostream>

using namespace hgl;

bool ParseShader(const OSString &filename);

void os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("a Test of Shader Parse.")<<std::endl;
    os_out<<HGL_COPYRIGHT_STRING_OS<<std::endl;
    
    os_out<<std::endl;

    if(argc!=2)
    {
        os_out<<OS_TEXT("example: ShaderParse 1.glsl")<<std::endl;
        return;
    }

    os_out<<OS_TEXT("GLSL File: ")<<argv[1]<<std::endl;

    ParseShader(argv[1]);

    return;
}
