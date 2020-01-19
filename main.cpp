#include"glsl2spv.h"
#include<hgl/type/StdString.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/log/LogInfo.h>
#include<iostream>
#include<iomanip>

using namespace hgl;

bool use_opengl_es=false;
bool use_subpass=false;

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

UTF8String MakeValueName(const ShaderDataFormat sdf)
{
    uint type,count;

    type=sdf>>2;
    count=sdf&3;

    return MakeValueName((ShaderDataType)type,count+1);
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

void OutputStringList(const UTF8StringList &sl)
{
    UTF8String **str=sl.GetDataList();

    for(int i=0;i<sl.GetCount();i++)
    {
        if((*str)->IsEmpty())
            std::cout<<std::endl;
        else
            std::cout<<(*str)->c_str()<<std::endl;

        ++str;
    }
}

void MakeShaderHeader(UTF8StringList &shader)
{
    if(use_opengl_es)
        shader.Add("#version es 320");
    else
        shader.Add("#version core 460");

    shader.Add(R"(/**
 * Copyright (c) 2018-2020, www.hyzgame.com
 *
 * Create by ShaderMaker
 */)");
}

void MakeWorldMatrix(UTF8StringList &shader)
{
shader.Add(R"(
layout(std430,binding=0,row_major) uniform WorldMatrix     // hgl/math/Math.h
{
    mat4 ortho;

    mat4 projection;
    mat4 inverse_projection;

    mat4 modelview;
    mat4 inverse_modelview;

    mat4 mvp;
    mat4 inverse_mvp;

    vec4 view_pos;
    vec2 resolution;
} world;)");
}

void MakePushConstant(UTF8StringList &shader)
{
shader.Add(R"(
layout(std430,push_constant,row_major) uniform Consts
{
    mat4 local_to_world;
    mat3 normal;
    vec3 object_position;
    vec3 object_size;
}pc;)");
}

void MakeGBufferAttribute(UTF8StringList &shader)
{
    ShaderAttribute *sa=attr_list.GetData();
    UTF8String type_name;
    
    for(int i=0;i<attr_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);

        shader.Add(U8_TEXT("    ")+type_name+U8_TEXT(" ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }
}

void MakeCustomCode(UTF8StringList &shader)
{
    shader.Add("//[Begin] Your code------------------------------------\n");
    shader.Add("//[End] Your code--------------------------------------");
}

bool SaveShaderToFile(const OSString &filename,const UTF8StringList &sl)
{
    io::FileOutputStream fos;

    if(!fos.CreateTrunc(filename))
        return(false);

    AutoDelete<io::UTF8TextOutputStream> tos=new io::UTF8TextOutputStream(&fos);

    tos->WriteText(sl);

    return(true);
}

void MakeGBufferFragmentShader(const OSString &output_filename)
{
    UTF8StringList shader;

    MakeShaderHeader(shader);
    MakeWorldMatrix(shader);
    MakePushConstant(shader);
    
    shader.Add("");

    ShaderAttribute *sa=gbuffer_list.GetData();
    UTF8String type_name;

    for(int i=0;i<gbuffer_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);

        shader.Add(U8_TEXT("layout(location=")+UTF8String(i)+U8_TEXT(") out ")+type_name+U8_TEXT(" ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }

    shader.Add(R"(
void main()
{)");

    MakeGBufferAttribute(shader);

    MakeCustomCode(shader);

    shader.Add(attribute_to_gbuffer);

    shader.Add(UTF8String("}"));

    OutputStringList(shader);
    
    SaveShaderToFile(output_filename+OS_TEXT("_gbuffer.frag"),shader);
}

void MakeCompositionVertexShader(const OSString &output_filename)
{
}

void MakeCompositionFragmentShader(const OSString &output_filename)
{
    UTF8StringList shader;

    MakeShaderHeader(shader);
    MakeWorldMatrix(shader);
    MakePushConstant(shader);

    shader.Add("");

    ShaderAttribute *sa=gbuffer_list.GetData();
    UTF8String type_name;

    //if(use_subpass)
    //    shader.Add(U8_TEXT("layout(input_attachment_index=0,binding=0) uniform subpassInput rb_depth;"));
    //else
    //    shader.Add(U8_TEXT("layout(binding=0) uniform sampler2D rb_depth;"));

    for(int i=0;i<gbuffer_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);
        
        if(use_subpass)
            shader.Add(U8_TEXT("layout(input_attachment_index=")+UTF8String(i+1)+U8_TEXT(",binding=")+UTF8String(i+1)+U8_TEXT(") uniform subpassInput si_")+UTF8String(sa->name)+U8_TEXT(";"));
        else
            shader.Add(U8_TEXT("layout(binding=")+UTF8String(i+1)+U8_TEXT(") uniform sampler2D ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }

//    shader.Add(R"(
//layout(location=0) out vec4 FragColor;
//
//highp vec3 RetrievePosition()
//{)");
//
//if(use_subpass)
//    shader.Add(R"(    highp vec4 clip    = vec4(gl_FragCoord.xy * world.resolution * 2.0 - 1.0, subpassLoad(rb_depth).x, 1.0);)");
//else
//    shader.Add(R"(    highp vec4 clip    = vec4(gl_FragCoord.xy * world.resolution * 2.0 - 1.0, texelFetch(rb_depth,gl_FragCoord.xy,0).x,1.0);)");
//
//shader.Add(R"(
//    highp vec4 world_w = world.inverse_projection * clip;
//    highp vec3 pos     = world_w.xyz / world_w.w;
//
//    return pos;
//}
//
//void main()
//{
//    highp vec3 Position=RetrievePosition();
//)");

    shader.Add(R"(
void main()
{)");

    MakeGBufferAttribute(shader);
    
    shader.Add("");
    shader.Add(gbuffer_to_attribute);
    shader.Add("");

    MakeCustomCode(shader);

    shader.Add(U8_TEXT("    FragColor="));
    shader.Add(U8_TEXT("}"));

    OutputStringList(shader);

    SaveShaderToFile(output_filename+OS_TEXT("_composition.frag"),shader);
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
        std::cout<<"Example: ShaderMaker output_name 1.gbuffer [/es]"<<std::endl;
        return(0);
    }

    util::CmdParse cp(argc,argv);

    if(cp.Find(OS_TEXT("/es"))>0)use_opengl_es=true;
//    if(cp.Find(OS_TEXT("/subpass"))>0)use_subpass=true;

    ParseGBufferConfig(argv[2]);

    MakeGBufferFragmentShader(argv[1]);

    MakeCompositionVertexShader(argv[1]);
    MakeCompositionFragmentShader(argv[1]);

    return(0);
}
