#include"ShaderLib.h"
#include<hgl/platform/Platform.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<iostream>

using namespace hgl;

enum class VaryingType
{
    Input,
    Output
};

class ShaderMaker
{
    shader_lib::XMLShader *xs;
    UTF8StringList shader_text;

    int ubo_binding;

private:

    bool CheckShader()
    {
        if(!shader_lib::CheckVarying(xs->in))return(false);
        if(!shader_lib::CheckVarying(xs->out))return(false);
        if(!shader_lib::CheckRawModule(xs->raw))return(false);
//        if(!shader_lib::CheckStruct(xs->struct_block))return(false);

        return(true);
    }

    void CreateHeader()
    {
        shader_text.Add(u8"/**\n"
                        u8" * the Shader created by ShaderMaker (" HGL_OFFICAL_WEB_U8 u8")\n"
                        u8" */\n"
                        u8"#version 450 core\n");
    }

    void OutComment(const UTF8String &str)
    {   
        u8char comment[81]=U8_TEXT("//-- ---------------------------------------------------------------------------");

        memcpy(comment+5,str.c_str(),str.Length());
        comment[5+str.Length()]=' ';

        shader_text.Add(comment);
    }
    
    UTF8String space_line;

    void OutEnter()
    {
        shader_text.Add(space_line);
    }

    int MakeVarying(const VaryingType &type,int binding,const UTF8String &vary_name)
    {
        shader_lib::VaryingConfig *vc=shader_lib::GetVarying(vary_name);

        if(!vc)return(-1);

        const int count=vc->GetCount();
        shader_lib::Varying **v=vc->GetData();

        for(int i=0;i<count;i++)
        {
            if(type==VaryingType::Input)
                shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") in ")+(*v)->type+U8_TEXT(" ")+(*v)->name+U8_TEXT(";"));
            else
                shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") out ")+(*v)->type+U8_TEXT(" out_")+(*v)->name+U8_TEXT(";"));

            ++v;
            ++binding;
        }

        return binding;
    }

    void MakeVarying(const VaryingType &type,const UTF8StringList &vary_list)
    {
        int binding=0;

        const int count=vary_list.GetCount();

        if(count<=0)return;

        const u8char *varying_type_str[]=
        {
            U8_TEXT("[in]"),
            U8_TEXT("[out]")
        };
        
        OutComment(U8_TEXT("Begin ")+UTF8String(varying_type_str[size_t(type)]));

        for(int i=0;i<count;i++)
            binding=MakeVarying(type,binding,vary_list.GetString(i));

        OutComment(U8_TEXT("End ")+UTF8String(varying_type_str[size_t(type)]));
        OutEnter();
    }

    void MakeStruct(const UTF8StringList &struct_list)
    {
        const int count=struct_list.GetCount();

        if(count<=0)return;
        
        OutComment(U8_TEXT("Begin ")+UTF8String::valueOf(count)+U8_TEXT(" structs"));
        for(int i=0;i<count;i++)
            shader_lib::AddStruct(shader_text,struct_list.GetString(i));
        OutComment(U8_TEXT("End structs"));
        OutEnter();
    }

    void MakeRaw()
    {
        const int count=xs->raw.GetCount();

        UTF8String rn;
        UTF8StringList *rm;

        for(int i=0;i<count;i++)
        {
            rn=xs->raw.GetString(i);
            rm=shader_lib::GetRawModule(rn);
            
            OutComment(U8_TEXT("Raw Begin [")+rn+U8_TEXT("]"));
            shader_text.Add(*rm);
            OutComment(U8_TEXT("Raw End [")+rn+U8_TEXT("]"));
            OutEnter();
        }
    }

    void MakeUniforms()
    {
        const int count=xs->uniforms.GetCount();
        shader_lib::Uniform **ubo=xs->uniforms.GetData();

        OutComment(U8_TEXT("Begin ")+UTF8String::valueOf(count)+U8_TEXT(" uniforms"));

        for(int i=0;i<count;i++)
        {
            shader_text.Add(U8_TEXT("layout(binding=")+UTF8String::valueOf(ubo_binding)+U8_TEXT(") uniform ")+(*ubo)->type_name+U8_TEXT(" ")+(*ubo)->value_name+U8_TEXT(";"));

            ++ubo_binding;
        }
        OutComment(U8_TEXT("End uniforms"));
        OutEnter();
    }

    void MakeMainFunc()
    {
        shader_text.Add(U8_TEXT("void main()\n{"));
        shader_text.Add(xs->main);
        shader_text.Add(U8_TEXT("}"));
    }

public:

    ShaderMaker(shader_lib::XMLShader *_xs)
    {
        xs=_xs;
        ubo_binding=0;
    }

    bool Make()
    {
        if(!CheckShader())return(false);
        
        CreateHeader();

        MakeVarying(VaryingType::Input,xs->in);

        MakeStruct(xs->struct_block);

        MakeUniforms();

        MakeRaw();

        MakeVarying(VaryingType::Output,xs->out);

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

