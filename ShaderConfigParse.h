#pragma once
#include<hgl/type/BaseString.h>
#include"ShaderDataType.h"
#include<iostream>
#include<iomanip>

using namespace hgl;

UTF8String SpaceChar = " \t;";

class ShaderConfigParse
{
protected:

    ShaderDataFormat ParseValue(UTF8String& value_name, const UTF8String& str)
    {
        if (str.Length() < 6)      //"vec 6;",最短就6个字符不可能更短了
            return 0;

        ShaderDataFormat sdf = 0;

        int type_start = str.FindExcludeChar(0, SpaceChar);

        if (type_start < 0)
            return 0;

        int type_count = str.FindChar(type_start, SpaceChar);

        if (type_count <= 0)
            return 0;

        UTF8String type_str = str.SubString(type_start, type_count);

        int value_start = str.FindExcludeChar(type_start + type_count, SpaceChar);

        if (value_start < 0)
            return 0;

        int value_count = str.FindChar(type_start + type_count + value_start, SpaceChar);

        if (value_count <= 0)
            return 0;

        value_name = str.SubString(type_start + type_count + value_start, value_count);

        sdf = ParseShaderType(type_str);

        std::cout << std::setw(2) << sdf << ":" << value_name.c_str() << std::endl;

        return sdf;
    }

public:

    ShaderConfigParse() {}
    virtual ~ShaderConfigParse() = default;

    virtual void Parse(const UTF8String&) = 0;
};//class ShaderConfigParse

inline bool IsSegmentFlag(const UTF8String& str)
{
    return(str.GetBeginChar() == '['
        && str.GetEndChar() == ']');
}
