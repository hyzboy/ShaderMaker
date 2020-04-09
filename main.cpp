﻿#include"ShaderDataType.h"
#include"glsl2spv.h"
#include"ShaderConfigParse.h"

#include<hgl/type/StdString.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/log/LogInfo.h>
#include<iostream>
#include<iomanip>

using namespace hgl;

bool use_opengl_es=false;
bool use_subpass=false;

ShaderAttributeList attr_list;
ShaderAttributeList gbuffer_list;

UTF8StringList attribute_to_gbuffer;
UTF8StringList gbuffer_to_attribute;

inline ShaderConfigParse *GetShaderSegment(const UTF8String &str)
{
    if(str.CaseComp("[attribute]"           )==0)return(new AttributeParse(&attr_list));
    if(str.CaseComp("[gbuffer]"             )==0)return(new AttributeParse(&gbuffer_list));
    if(str.CaseComp("[attribute_to_gbuffer]")==0)return(new CodeLog(&attribute_to_gbuffer));
    if(str.CaseComp("[gbuffer_to_attribute]")==0)return(new CodeLog(&gbuffer_to_attribute));

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
 * Copyright (c) 2018-2020 www.hyzgame.com
 *
 * Create by ShaderMaker
 */)");
}

void MakeWorldMatrix(UTF8StringList &shader)
{
shader.Add(R"(
layout(binding=0) uniform WorldMatrix     // hgl/math/Math.h
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
layout(push_constant) uniform Consts
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
    UTF8StringList shader;

    MakeShaderHeader(shader);

    shader.Add(R"(
layout(location = 0) in vec2 Vertex;

layout(location = 0) out vec2 vs_out_position;

void main()
{
    gl_Position=vec4(Vertex,0.0,1.0);

    vs_out_position=(Vertex+1.0)/2.0;
})");

    OutputStringList(shader);

    SaveShaderToFile(output_filename+OS_TEXT("_composition.vert"),shader);
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

    shader.Add("");
    shader.Add(U8_TEXT("layout(location=0) in vec2 vs_out_position;"));
    shader.Add(U8_TEXT("layout(location=0) out vec4 FragColor;"));

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
