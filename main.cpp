#include"glsl2spv.h"
#include<hgl/type/StdString.h>
#include<hgl/type/StringList.h>
#include<hgl/log/LogInfo.h>
#include<iostream>
#include<iomanip>

using namespace hgl;

constexpr size_t MAX_SHADER_VALUE_NAME_LENGTH=32;

enum class ShaderDataType
{
    Bool=0,
    Float,
    Int,
    Uint,
    Double,

    BEGIN_RANGE =Bool,
    END_RANGE   =Double,
    RANGE_SIZE  =(END_RANGE-BEGIN_RANGE+1)
};//

using ShaderDataFormat=uint32;

inline ShaderDataFormat MakeShaderDataFormat(const ShaderDataType type,const uint count)
{
    if(type<ShaderDataType::BEGIN_RANGE
     ||type>ShaderDataType::END_RANGE)return 0;

    if(count<1||count>4)return 0;

    return((uint(type)<<2)|(count-1));
}

UTF8String MakeValueName(const ShaderDataType type,const uint count)
{
    constexpr char type_name[][8]=
    {
        "bool",
        "float",
        "int",
        "uint",
        "double"
    };

    constexpr char type_vec_name[][8]=
    {
        "bvec",
        "vec",
        "ivec",
        "uvec",
        "dvec"
    };

    if(type<ShaderDataType::BEGIN_RANGE
     ||type>ShaderDataType::END_RANGE)
        return("[ERROR ShaderDataType]");

    if(count<1||count>4)
        return("[ERROR Type component count]");

    if(count==1)
        return type_name[uint(type)];

    return UTF8String(type_vec_name[uint(type)])+UTF8String(count);
}

const ShaderDataFormat ParseShaderType(const UTF8String &str)
{
    const char first_char=str.GetBeginChar();
    const char end_char=str.GetEndChar();

    if(first_char=='f')return MakeShaderDataFormat(ShaderDataType::Float,1);

    uint count=0;

    if(end_char>='2'&&end_char<='4')        //vec
        count=end_char-'0';
    else 
        count=1;

    if(first_char=='v')return MakeShaderDataFormat(ShaderDataType::Float,   count);else
    if(first_char=='b')return MakeShaderDataFormat(ShaderDataType::Bool,    count);else
    if(first_char=='i')return MakeShaderDataFormat(ShaderDataType::Int,     count);else
    if(first_char=='u')return MakeShaderDataFormat(ShaderDataType::Uint,    count);else
    if(first_char=='d')return MakeShaderDataFormat(ShaderDataType::Double,  count);else
        return 0;   
}
    
struct ShaderAttribute
{
    ShaderDataFormat format;
    char name[MAX_SHADER_VALUE_NAME_LENGTH+1];
};

using ShaderAttributeList=List<ShaderAttribute>;

ShaderAttributeList attr_list;
ShaderAttributeList gbuffer_list;

UTF8StringList attribute_to_gbuffer;
UTF8StringList gbuffer_to_attribute;

UTF8String ShaderTypeChar="bolfatinudev";
UTF8String SpaceChar=" \t;";

class ShaderConfigParse
{
protected:

    ShaderDataFormat ParseValue(UTF8String &value_name,const UTF8String &str)
    {
        if(str.Length()<6)      //"vec 6;",最短就6个字符不可能更短了
            return 0;

        ShaderDataFormat sdf=0;
        
        int type_start=str.FindExcludeChar(0,SpaceChar);

        if(type_start<0)
            return 0;
        
        int type_count=str.FindChar(type_start,SpaceChar);

        if(type_count<=0)
            return 0;

        UTF8String type_str=str.SubString(type_start,type_count);

        int value_start=str.FindExcludeChar(type_start+type_count,SpaceChar);

        if(value_start<0)
            return 0;

        int value_count=str.FindChar(type_start+type_count+value_start,SpaceChar);

        if(value_count<=0)
            return 0;

        value_name=str.SubString(type_start+type_count+value_start,value_count);

        sdf=ParseShaderType(type_str);

        std::cout<<std::setw(2)<<sdf<<":"<<value_name.c_str()<<std::endl;

        return sdf;
    }

public:

    ShaderConfigParse(){}
    virtual ~ShaderConfigParse()=default;

    virtual void Parse(const UTF8String &)=0;
};//

class AttributeParse:public ShaderConfigParse
{
public:

    AttributeParse():ShaderConfigParse()
    {
        std::cout<<"[Attribute]"<<std::endl;
    }

    void Parse(const UTF8String &str) override
    {
        UTF8String value_name;
        ShaderDataFormat sdf;

        sdf=ParseValue(value_name,str);

        if(sdf==0)
        {
            std::cerr<<"[Attribute] ShaderDataType parse error,str=\""<<str.c_str()<<"\""<<std::endl;
            return;
        }

        ShaderAttribute sa;

        sa.format=sdf;
        hgl::strcpy(sa.name,sizeof(sa.name),value_name.c_str());

        attr_list.Add(sa);        
    }
};

class GBufferParse:public ShaderConfigParse
{
public:

    GBufferParse():ShaderConfigParse()
    {
        std::cout<<"[GBuffer]"<<std::endl;
    }
    
    void Parse(const UTF8String &str) override
    {
        UTF8String value_name;
        ShaderDataFormat sdf;

        sdf=ParseValue(value_name,str);

        if(sdf==0)
        {
            std::cerr<<"[GBuffer] ShaderDataType parse error,str=\""<<str.c_str()<<"\""<<std::endl;
            return;
        }

        ShaderAttribute sa;

        sa.format=sdf;
        hgl::strcpy(sa.name,sizeof(sa.name),value_name.c_str());

        gbuffer_list.Add(sa);
    }
};

class AttributeToGBufferParse:public ShaderConfigParse
{
public:

    AttributeToGBufferParse():ShaderConfigParse()
    {
        std::cout<<"[AttributeToGBuffer] "<<std::endl;
    }

    void Parse(const UTF8String &str) override
    {
        attribute_to_gbuffer.Add(str);
    }
};

class GBufferToAttributeParse:public ShaderConfigParse
{
public:

    GBufferToAttributeParse():ShaderConfigParse()
    {
        std::cout<<"[GBufferToAttribute] "<<std::endl;
    }

    void Parse(const UTF8String &str) override
    {
        gbuffer_to_attribute.Add(str);
    }
};

inline bool IsSegmentFlag(const UTF8String &str)
{
    return(str.GetBeginChar()   =='['
         &&str.GetEndChar()     ==']');
}

inline ShaderConfigParse *GetShaderSegment(const UTF8String &str)
{
    if(str.CaseComp("[attribute]"           )==0)return(new AttributeParse());
    if(str.CaseComp("[gbuffer]"             )==0)return(new GBufferParse());
    if(str.CaseComp("[attribute_to_gbuffer]")==0)return(new AttributeToGBufferParse());
    if(str.CaseComp("[gbuffer_to_attribute]")==0)return(new GBufferToAttributeParse());

    return nullptr;
}

bool ParseGBufferConfig(const OSString &filename)
{
    UTF8StringList gbfile;

    ShaderConfigParse *cur_parse=nullptr;

    const int lines=LoadStringListFromTextFile(gbfile,filename);

    if(lines<=4)
        return(false);

    LOG_INFO(OS_TEXT("file \"")+filename+OS_TEXT("\" total ")+OSString(lines)+OS_TEXT(" lines.") HGL_LINE_END);

    for(int i=0;i<lines;i++)
    {
        const UTF8String &str=gbfile.GetString(i);

        if(str.IsEmpty())continue;

        if(IsSegmentFlag(str))
        {
            if(cur_parse)
                delete cur_parse;

            cur_parse=GetShaderSegment(str);
            continue;
        }

        cur_parse->Parse(str);
    }

    return(true);
}

#if HGL_OS == HGL_OS_Windows
int wmain(int argc,wchar_t **argv)
#else
int main(int argc,char **argv)
#endif//
{
    std::cout<<"ShaderMaker 1.0"<<std::endl;
    std::cout<<"Copyright (C) www.hyzgame.com"<<std::endl;
    std::cout<<std::endl;

    if(argc<2)
    {
        std::cout<<"Example: ShaderMaker 1.gbuffer"<<std::endl;
        return(0);
    }

    ParseGBufferConfig(argv[1]);

    return(0);
}
