#pragma once

#include"ShaderSourceLocation.h"
#include<hgl/type/List.h>
#include<hgl/type/StringList.h>

using namespace hgl;

struct ShaderFunction:public ShaderSourceLocation
{
    UTF8String name;

    UTF8StringList identify_list;           ///<用到的标识符列表(可能是变量、结构、函数、const变量等等)
};
