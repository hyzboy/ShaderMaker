#include"ShaderLib.h"
#include"GLSLCompiler.h"
#include<iostream>

using namespace hgl;

namespace shader_lib
{
    bool SaveMaterial(const OSString &filename,XMLMaterial *);
    XMLMaterial *LoadXMLMaterial(const OSString &filename);
}//namespace shader_lib

int os_main(int argc,os_char **argv)
{
    std::cout<<"MaterialCompiler v1.0 (offical web: "<<HGL_OFFICAL_WEB<<")"<<std::endl<<std::endl;

    if(argc<2)
    {
        std::cout<<"example: MaterialCompiler <shader_libs path> [material filename]"<<std::endl<<std::endl;
        return 0;
    }

    if(!glsl_compiler::Init())
    {
        std::cerr<<"Init GLSLCompiler failed!"<<std::endl;
        return -1;
    }

    os_out<<"shader_libs path: "<<argv[1]<<std::endl;

    shader_lib::LoadFromFolder(argv[1]);

    if(argc<=2)return(0);

    const OSString filename=argv[2];

    os_out<<"material filename: "<<argv[2]<<std::endl;

    shader_lib::XMLMaterial *mat=shader_lib::LoadXMLMaterial(filename);
    
    if(mat)
    {
        const OSString mat_filename=filesystem::TrimFileExtName(filename,true);

        shader_lib::SaveMaterial(mat_filename,mat);
    }

    glsl_compiler::Close();

    return 0;
}
