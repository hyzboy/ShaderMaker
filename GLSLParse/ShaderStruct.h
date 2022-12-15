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

public:

    const int Comp(const ShaderVarying &sv)const
    {
        int result;

        result=type.Comp(sv.type);
        if(result)return result;

        result=name.Comp(sv.name);
        if(result)return result;

        result=interpolation.Comp(sv.interpolation);
        if(result)return result;

        return 0;
    }

    CompOperator(const ShaderVarying &,Comp);
};//

struct ShaderStruct:public ShaderSourceLocation
{
    UTF8String name;
    UTF8String comment;
    ObjectList<ShaderVarying> varyings;

    bool all_origin_type;           //是否全原生类型(shader中的输入输出不可以有结构套结构，也许能用，但我们不支持)

public:

    const int Comp(const ShaderStruct &ss)const
    {
        int result;

        result=name.Comp(ss.name);
        if(result)return result;

        result=varyings.GetCount()-ss.varyings.GetCount();
        if(result)return result;

        for(int i=0;i<varyings.GetCount();i++)
        {
            result=varyings[i]->Comp(*ss.varyings[i]);
            if(result)return result;
        }

        return 0;
    }

    CompOperator(const ShaderStruct &,Comp);
};//
