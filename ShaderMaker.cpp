#include"ShaderLib.h"
#include"GLSLCompiler.h"
#include<hgl/platform/Platform.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<iostream>

using namespace hgl;

namespace shader_lib
{
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

            if(xs->shader_type==shader_lib::ssbGeometry)
            {
                shader_text.Add(u8"layout("+xs->geom.in+u8") in;");
                shader_text.Add(u8"layout("+xs->geom.out+u8",max_vertices="+UTF8String::valueOf(xs->geom.max_vertices)+u8") out;\n");
            }
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
                {
                    if(xs->shader_type==shader_lib::ssbGeometry)
                        shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") in ")+(*v)->type+U8_TEXT(" ")+(*v)->name+U8_TEXT("[];"));
                    else
                        shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") in ")+(*v)->type+U8_TEXT(" ")+(*v)->name+U8_TEXT(";"));
                }
                else
                if(xs->shader_type==shader_lib::ssbFragment)                
                    shader_text.Add(U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(") out ")+(*v)->type+U8_TEXT(" ")+(*v)->name+U8_TEXT(";"));
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

            UTF8String front;

            for(int i=0;i<count;i++)
            {
                if((*ubo)->binding>ubo_binding)
                    ubo_binding=(*ubo)->binding;

                front=U8_TEXT("layout(binding=")+UTF8String::valueOf(ubo_binding)+U8_TEXT(") uniform");

                shader_lib::AddStruct(shader_text,front,(*ubo)->type_name,(*ubo)->value_name);

                ++ubo_binding;
                ++ubo;
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

        ~ShaderMaker()=default;

        bool Make(const UTF8String &ext_name)
        {
            if(!CheckShader())return(false);
            
            xs->shader_type=glsl_compiler::GetType(ext_name.c_str());

            const char *ShaderFileTypeName[]={"vert","tesc","tese","geom","frag","comp","mesh","task","rgen","rint","rahit","rchit","rmiss","rcall"};

            ubo_binding=string_serial_from_list(ShaderFileTypeName,ext_name.c_str())*16;
        
            CreateHeader();

            MakeVarying(VaryingType::Input,xs->in);

            MakeUniforms();

            MakeRaw();

            MakeVarying(VaryingType::Output,xs->out);

            MakeMainFunc();

            return xs->SetShaderSource(shader_text);
        }
    };//class ShaderMaker

    bool XMLShaderMaker(const OSString &filename,shader_lib::XMLShader *xs)
    {
        if(!xs)return(false);

        ShaderMaker sm(xs);

        /*  example

                filename:   1.vert.xml
                short_name: 1.vert
                ext_name:   vert
                glsl_name:  1.vert.glsl
                spv:        1.vert.spv
                shader:     1.vert.shader
        */

        const OSString short_name=filesystem::TrimFileExtName(filename,true);
        const UTF8String ext_name=to_u8(filesystem::ClipFileExtName(short_name,false));

        std::cout<<"------ Generate "<<ext_name.c_str()<<" shader ------"<<std::endl;

        if(!sm.Make(ext_name))
        {
            const OSString glsl_filename=short_name+OS_TEXT(".glsl");

            os_err<<OS_TEXT("Error GLSL: ")<<glsl_filename.c_str()<<std::endl;

            xs->SaveToGLSL(glsl_filename);
            return(false);
        }
        
        //xs->SaveToSPV(short_name+OS_TEXT(".spv"));
        //xs->SaveToShader(short_name+OS_TEXT(".shader"));

        return(true);
    }
}//namespace shader_lib