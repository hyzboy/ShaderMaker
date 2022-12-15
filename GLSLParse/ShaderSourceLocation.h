#pragma once

#include<hgl/type/String.h>

using namespace hgl;

struct ShaderSourceLocation
{
    UTF8String module_name;             ///<所属那个模块

    UTF8String file;
    uint line;

public:

    const int Comp(const ShaderSourceLocation &ssl) const
    {
        int result;
            
        result=module_name.Comp(ssl.module_name);
        if(result)return result;

        result=file.Comp(ssl.file);
        if(result)return result;

        return line-ssl.line;
    }

    CompOperator(const ShaderSourceLocation &,Comp)
};
