#pragma once

#include<hgl/type/String.h>

using namespace hgl;

struct ShaderSourceLocation
{
    UTF8String module_name;             ///<所属那个模块

    UTF8String file;
    uint line;
};
