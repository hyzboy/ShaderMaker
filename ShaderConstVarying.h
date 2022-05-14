#pragma once

#include"ShaderStruct.h"

struct ShaderConstVarying:public ShaderVarying,public ShaderSourceLocation
{
public:
    
    const int Comp(const ShaderConstVarying &scv)const
    {
        int result;
        
        result=ShaderSourceLocation::Comp(scv);
        if(result)return result;

        result=ShaderVarying::Comp(scv);
        return result;
    }

    CompOperator(const ShaderConstVarying &,Comp);
};
