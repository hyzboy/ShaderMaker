#pragma once
#include<hgl/type/StringList.h>
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

class AttributeParse :public ShaderConfigParse
{
    ShaderAttributeList *attr_list;

public:

    AttributeParse(ShaderAttributeList *sal) :ShaderConfigParse()
    {
        attr_list=sal;
    }

    void Parse(const UTF8String& str) override
    {
        UTF8String value_name;
        ShaderDataFormat sdf;

        sdf = ParseValue(value_name, str);

        if (sdf == 0)
        {
            std::cerr << "[AttributeParse] ShaderDataType parse error,str=\"" << str.c_str() << "\"" << std::endl;
            return;
        }

        ShaderAttribute sa;

        sa.format = sdf;
        hgl::strcpy(sa.name, sizeof(sa.name), value_name.c_str());

        attr_list->Add(sa);
    }
};//class AttributeParse :public ShaderConfigParse

class CodeLog :public ShaderConfigParse
{
    UTF8StringList *code_list;

public:

    CodeLog(UTF8StringList *sl) :ShaderConfigParse()
    {
        code_list=sl;
    }

    void Parse(const UTF8String& str) override
    {
        code_list->Add(str);
    }
};//class CodeLog :public ShaderConfigParse
