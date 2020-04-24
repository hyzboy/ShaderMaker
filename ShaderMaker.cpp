#include"ShaderMaker.h"
#include"ShaderLib.h"
#include<iostream>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>

namespace
{
    static int si_header = -1;
    static int si_comment = -1;
    static int si_push_constant = -1;

    static int si_ubo_world_matrix = -1;

    static int si_fs_rectangle_vs = -1;
}

ShaderMaker::ShaderMaker(const RenderPlatformConfig &rpc)
{
    rp_cfg=rpc;

    si_header           = shader_lib::Load(rpc.mobile?OS_TEXT("header_mobile.glsl")
                                                     :OS_TEXT("header_desktop.glsl"));
    si_comment          = shader_lib::Load(OS_TEXT("header_comment.glsl"));
    si_push_constant    = shader_lib::Load(OS_TEXT("push_constant_3d.glsl"));
    si_ubo_world_matrix = shader_lib::Load(OS_TEXT("UBO_WorldMatrix.glsl"));
    si_fs_rectangle_vs  = shader_lib::Load(OS_TEXT("full_screen_rectangle.vert.glsl"));

    shader_lib::AddTo(shader_source, si_header);
    shader_lib::AddTo(shader_source, si_comment);
}

ShaderMaker::~ShaderMaker()
{
    std::cout<<std::endl;
    std::cout<<"Shader: "<<rp_cfg.name.c_str()<<std::endl;

    UTF8String** str = shader_source.GetDataList();

    for (int i = 0; i < shader_source.GetCount(); i++)
    {
        if ((*str)->IsEmpty())
            std::cout << std::endl;
        else
            std::cout << (*str)->c_str() << std::endl;

        ++str;
    }

    std::cout << std::endl;

    shader_lib::Clear();
}

bool ShaderMaker::Save(const OSString &filename)
{
    io::FileOutputStream fos;

    if(!fos.CreateTrunc(filename))
        return(false);

    AutoDelete<io::UTF8TextOutputStream> tos=new io::UTF8TextOutputStream(&fos);

    tos->WriteText(shader_source);

    return(true);
}

void ShaderMaker::MakeCustomCode()
{
    shader_source.Add(R"(//[Begin] Your code------------------------------------

//[End] Your code--------------------------------------)");
}
