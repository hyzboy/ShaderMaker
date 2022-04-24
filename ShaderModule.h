#pragma once

#include"ShaderConstVarying.h"
#include"ShaderFunction.h"
#include<hgl/type/Map.h>

struct ShaderModule
{
    UTF8String name;

    OSString filename;
    double last_write_file;

    bool is_ok;     ///<是否可用

    Map<UTF8String,ShaderConstVarying>  scv_list;
    Map<UTF8String,ShaderStruct>        struct_list;
    Map<UTF8String,ShaderFunction>      func_list;
};//struct ShaderModule
