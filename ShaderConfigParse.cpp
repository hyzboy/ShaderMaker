#include"ShaderConfig.h"
#include<hgl/log/LogInfo.h>
#include<hgl/type/StringList.h>
#include<iostream>
#include<iomanip>

namespace
{
    UTF8String SpaceChar = " \t;";

    ShaderDataFormat ParseValue(UTF8String &value_name, const UTF8String &str)
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

    bool IsSegmentFlag(const UTF8String &str)
    {
        if(str.GetBeginChar()   !='[')return(false);
        if(str.GetEndChar()     !=']')return(false);

        return(true);
    }

    class ParseBase
    {
    public:

        ParseBase() {}
        virtual ~ParseBase() = default;

        virtual void Parse(const UTF8String&) = 0;
    };//class ShaderConfigParse

    class AttributeParse :public ParseBase
    {
        ShaderAttributeList *attr_list;

    public:

        AttributeParse(ShaderAttributeList *sal) :ParseBase()
        {
            attr_list = sal;
        }

        void Parse(const UTF8String &str) override
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

    class CodeLog :public ParseBase
    {
        UTF8StringList *code_list;

    public:

        CodeLog(UTF8StringList *sl) :ParseBase()
        {
            code_list = sl;
        }

        void Parse(const UTF8String &str) override
        {
            code_list->Add(str);
        }
    };//class CodeLog :public ShaderConfigParse

    ParseBase *GetShaderSegment(const UTF8String &str, ShaderConfig *cfg)
    {
        if (str.CaseComp("[depend]"     ) == 0)return(new AttributeParse(&cfg->attr_list));
        if (str.CaseComp("[attr]"       ) == 0)return(new AttributeParse(&cfg->attr_list));
        if (str.CaseComp("[framebuffer]") == 0)return(new AttributeParse(&cfg->fb_list));
        if (str.CaseComp("[attr_to_fb]" ) == 0)return(new CodeLog(&cfg->attr2fb));
        if (str.CaseComp("[gb_to_attr]" ) == 0)
        {cfg->deferred=true;                   return(new CodeLog(&cfg->gb2attr));}

        return nullptr;
    }
}//namespace

ShaderConfig *LoadShaderConfig(const OSString &filename)
{
    UTF8StringList cfg_list;

    ParseBase *cur_parse = nullptr;

    const int lines = LoadStringListFromTextFile(cfg_list, filename);

    if (lines <= 4)
        return(false);

    LOG_INFO(OS_TEXT("file \"") + filename + OS_TEXT("\" total ") + OSString::valueOf(lines) + OS_TEXT(" lines.") HGL_LINE_END);

    ShaderConfig *cfg=new ShaderConfig;

    for (int i = 0; i < lines; i++)
    {
        const UTF8String &str = cfg_list.GetString(i);

        if (str.IsEmpty())continue;

        if(str.GetBeginChar()=='/')continue;        //注释跳过

        int pos=str.FindExcludeChar(" \t");

        if (IsSegmentFlag(str))
        {
            if (cur_parse)
                delete cur_parse;

            cur_parse = GetShaderSegment(str,cfg);
            continue;
        }

        cur_parse->Parse(str);
    }

    return(cfg);
}
