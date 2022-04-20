#pragma once

#include"ShaderSourceLocation.h"
#include<hgl/type/List.h>

using namespace hgl;

struct ShaderVarying
{
    UTF8String type;
    UTF8String name;
    UTF8String interpolation;
    UTF8String comment;
};//

struct ShaderStruct
{
    UTF8String name;
    UTF8String comment;
    ObjectList<ShaderVarying> varyings;

    ShaderSourceLocation location;
};
