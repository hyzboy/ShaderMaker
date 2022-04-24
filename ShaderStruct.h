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

    bool is_origin_type;            //是否原生类型
};//

struct ShaderStruct:public ShaderSourceLocation
{
    UTF8String name;
    UTF8String comment;
    ObjectList<ShaderVarying> varyings;

    bool all_origin_type;           //是否全原生类型(shader中的输入输出不可以有结构套结构，也许能用，但我们不支持)
};
