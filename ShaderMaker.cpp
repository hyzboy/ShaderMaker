#include"ShaderLib.h"
#include<hgl/platform/Platform.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<iostream>

using namespace hgl;

class ShaderMaker
{
    shader_lib::XMLShader *xs;
    UTF8StringList shader_text;

private:

    bool CheckShader()
    {
        if(!shader_lib::CheckVarying(xs->in))return(false);
        if(!shader_lib::CheckVarying(xs->out))return(false);

        return(true);
    }

    void CreateHeader()
    {
        shader_text.Add(u8"/**\n"
                        u8" * the Shader created by ShaderMaker (" HGL_OFFICAL_WEB_U8 u8")\n"
                        u8" */\n"
                        u8"#version 450 core\n");
    }

    int MakeVarying(const UTF8String &type,int binding,const UTF8String &vary_name)
    {
        shader_lib::VaryingConfig *vc=shader_lib::GetVarying(vary_name);

        if(!vc)return(-1);

        const int count=vc->GetCount();
        shader_lib::Varying **v=vc->GetData();

        for(int i=0;i<count;i++)
        {
            shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") ")+type+U8_TEXT(" ")+(*v)->type+U8_TEXT(" ")+type+U8_TEXT("_")+(*v)->name+U8_TEXT(";"));

            ++v;
            ++binding;
        }

        return binding;
    }

    void MakeVarying(const UTF8String &type,const UTF8StringList &vary_list)
    {
        int binding=0;

        const int count=vary_list.GetCount();

        if(count<=0)return;

        for(int i=0;i<count;i++)
            binding=MakeVarying(type,binding,vary_list.GetString(i));
    }

    void MakeMainFunc()
    {
        shader_text.Add(U8_TEXT("\nvoid main()\n{"));
        shader_text.Add(xs->main);
        shader_text.Add(U8_TEXT(")"));
    }

public:

    ShaderMaker(shader_lib::XMLShader *_xs)
    {
        xs=_xs;
    }

    bool Make()
    {
        if(!CheckShader())return(false);
        
        CreateHeader();

        MakeVarying(U8_TEXT("in"),xs->in);

        MakeVarying(U8_TEXT("out"),xs->out);

        MakeMainFunc();

        return(true);
    }

    bool Save(const OSString &filename)
    {
        io::FileOutputStream fos;

        if(!fos.CreateTrunc(filename))
            return(false);

        io::UTF8TextOutputStream tos(&fos);

        tos.WriteText(shader_text);
        return(true);
    }   
};//class ShaderMaker

int os_main(int argc,os_char **argv)
{
    std::cout<<"ShaderMaker v1.0 (offical web: "<<HGL_OFFICAL_WEB<<")"<<std::endl<<std::endl;

    if(argc<2)
    {
        std::cout<<"example: ShaderMaker <shader_libs path> [material filename]"<<std::endl<<std::endl;
        return 0;
    }

    os_out<<"shader_libs path: "<<argv[1]<<std::endl;

    shader_lib::LoadFromFolder(argv[1]);

    if(argc<=2)return(0);

    {
        const OSString xs_fn=argv[2];

        AutoDelete<shader_lib::XMLShader> xs=shader_lib::LoadXMLShader(xs_fn);

        if(!xs)return(-1);

        ShaderMaker sm(xs);

        if(!sm.Make())return(-2);

        return sm.Save(xs_fn+OS_TEXT(".glsl"));
    }

    return 0;
}

