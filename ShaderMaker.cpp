#include"ShaderLib.h"
#include<hgl/platform/Platform.h>
#include<iostream>

using namespace hgl;

int main(int argc,os_char **argv)
{
    std::cout<<"ShaderMaker v1.0 (offical web: "<<HGL_OFFICAL_WEB<<")"<<std::endl<<std::endl;

    if(argc<2)
    {
        std::cout<<"example: ShaderMaker <shader_libs path> [material filename]"<<std::endl<<std::endl;
        return 0;
    }

    std::cout<<"shader_libs path: "<<argv[1]<<std::endl;

    shader_lib::Init(argv[1]);

    return 0;
}