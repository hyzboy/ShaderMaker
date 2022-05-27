#include"TP_ShaderLibrary.h"

TPShaderLibrary::TPShaderLibrary()
{
    this->addTab(varying=new TPVarying,"Varying");
    this->addTab(rawglsl=new TPRawGLSL,"Raw GLSL");
    this->addTab(shader_struct=new TPStruct,"Struct");
    this->addTab(shader_module=new TPModule,"Module");
}

TPShaderLibrary::~TPShaderLibrary()
{
}
