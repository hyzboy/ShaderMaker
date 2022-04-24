#pragma once

#include"ShaderStruct.h"

struct ShaderConstVarying:public ShaderVarying,public ShaderSourceLocation
{
    ShaderSourceLocation location;
};
