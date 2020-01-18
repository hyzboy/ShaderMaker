#include"glsl2spv.h"
#include<hgl/type/StdString.h>
#include<hgl/type/StringList.h>
#include<iostream>

using namespace hgl;

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
}

enum class ShaderSegment
{
    None=0,

    Attribute,
    Output,
    AttributeToGBuffer,
    GBufferToAttribute


};

inline bool IsSegmentFlag(const UTF8String &str)
{
    return(str.GetBeginChar()   =='['
         &&str.GetEndChar()     ==']');
}

inline const ShaderSegment &GetShaderSegment(const UTF8String &str)
{
    if(str.CaseComp("[attribute]"           )==0)return ShaderSegment::Attribute;
    if(str.CaseComp("[output]"              )==0)return ShaderSegment::Output;
    if(str.CaseComp("[attribute_to_gbuffer]")==0)return ShaderSegment::AttributeToGBuffer;
    if(str.CaseComp("[gbuffer_to_attribute]")==0)return ShaderSegment::GBufferToAttribute;

    return ShaderSegment::None;
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

    UTF8StringList gbfile;

    ShaderSegment cur_segment=ShaderSegment::None;

    const int lines=LoadStringListFromTextFile(gbfile,argv[1]);

    std::cout<<"file total lines="<<lines<<std::endl;

    for(int i=0;i<lines;i++)
    {
        const UTF8String &str=gbfile.GetString(i);

        if(IsSegmentFlag(str))
            cur_segment=GetShaderSegment(str);

        
    }

    return(0);
}
