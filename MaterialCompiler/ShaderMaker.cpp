#include"ShaderLibrary/ShaderLib.h"
#include"GLSLCompiler/GLSLCompiler.h"
#include<hgl/platform/Platform.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<iostream>

using namespace hgl;

namespace shader_lib
{
//    constexpr char *ShaderFileTypeName[]={"vert","tesc","tese","geom","frag","comp","mesh","task","rgen","rint","rahit","rchit","rmiss","rcall"};

    enum class VaryingType
    {
        Input,
        Output
    };

    class ShaderMaker
    {
        MaterialStat *mtl_stat;

        InfoOutput *info_output;

        shader_lib::XMLShader *xs;
        UTF8StringList shader_text;

    private:

        void OutError(const UTF8String &str)
        {
            info_output->colorWrite("red",str);
        }

    private:

        bool ExpendDepend(const UTF8String &module_name)
        {
            XMLShaderModule *xsm=GetShaderModule(module_name);

            if(!xsm)
            {
                OutError("Can't find module: "+module_name);
                return(false);
            }

            for(const UTF8String *name:xsm->depend_raw_list)
                xs->raw.Add(*name);

            return(true);
        }

        bool ExpendDepend()
        {
            info_output->colorWriteln("blue","Expend depends...");

            for(const UTF8String *module_name:xs->modules)
                if(!ExpendDepend(*module_name))
                    return(false);

            return(true);
        }

        bool CheckShader()
        {
            if(!shader_lib::CheckVarying(xs->in,info_output))return(false);
            if(!shader_lib::CheckVarying(xs->out,info_output))return(false);
            if(!shader_lib::CheckRawModule(xs->raw,info_output))return(false);
    //        if(!shader_lib::CheckStruct(xs->struct_block))return(false);

            return(true);
        }

        void CreateHeader()
        {
            shader_text.Add(u8"// the Shader was created by the MaterialWriter (" HGL_OFFICAL_WEB_U8 u8")\n"
                            u8"\n"
                            u8"#version 460 core\n");

            if(xs->shader_type==shader_lib::ssbGeometry)
            {
                shader_text.Add(u8"layout("+xs->geom.in+u8") in;");
                shader_text.Add(u8"layout("+xs->geom.out+u8",max_vertices="+UTF8String::valueOf(xs->geom.max_vertices)+u8") out;\n");
            }
        }

        void OutComment(const UTF8String &str)
        {
            u8char comment[]=U8_TEXT("//-- ---------------------------------------------------------------------------");

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

            UTF8String layout;
            UTF8String in_value_name;

            for(int i=0;i<count;i++)
            {
                layout=U8_TEXT("layout(location=")+UTF8String::valueOf(binding)+U8_TEXT(")");

                if(type==VaryingType::Input)
                {
                    in_value_name=U8_TEXT(" in ")+(*v)->type+U8_TEXT(" ")+(*v)->name;

                    if(xs->shader_type==shader_lib::ssbGeometry)
                        shader_text.Add(layout+                     in_value_name+U8_TEXT("[];"));
                    else
                    if(xs->shader_type==shader_lib::ssbFragment&&(*v)->interpolation.Length()>0)
                        shader_text.Add(layout+(*v)->interpolation+ in_value_name+U8_TEXT(";"));
                    else
                        shader_text.Add(layout+                     in_value_name+U8_TEXT(";"));
                }
                else
                {
                    layout+=U8_TEXT(" out ")+(*v)->type;

                    if(xs->shader_type==shader_lib::ssbFragment)
                        shader_text.Add(layout+U8_TEXT(" ")+    (*v)->name+U8_TEXT(";"));
                    else
                        shader_text.Add(layout+U8_TEXT(" out_")+(*v)->name+U8_TEXT(";"));
                }

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

            if(count<=0)return;

            UTF8StringList *rm;

            for(const UTF8String *rn:xs->raw)
            {
                rm=shader_lib::GetRawModule(*rn);
            
                OutComment(U8_TEXT("Raw Begin [")+(*rn)+U8_TEXT("]"));
                shader_text.Add(*rm);
                OutComment(U8_TEXT("Raw End [")+(*rn)+U8_TEXT("]"));
                OutEnter();

                ++rn;
            }
        }

        void MakeModuleCodes()
        {            
            for(const UTF8String *module_name:xs->modules)
            {
                XMLShaderModule *xsm=GetShaderModule(*module_name);

                if(xsm)
                {
                    OutComment(U8_TEXT("Module codes Begin [")+(*module_name)+U8_TEXT("]"));
                    shader_text.Add(xsm->codes);
                    OutComment(U8_TEXT("Module codes End [")+(*module_name)+U8_TEXT("]"));
                    OutEnter();
                }
            }
        }

        void MakeUniforms()
        {
            const int count=xs->uniforms.GetCount();

            if(count<=0)return;

            shader_lib::Uniform **ubo=xs->uniforms.GetData();

            OutComment(U8_TEXT("Begin ")+UTF8String::valueOf(count)+U8_TEXT(" uniforms"));

            UTF8String front;

            for(int i=0;i<count;i++)
            {
                if((*ubo)->set_number==-1)
                {
                    front=U8_TEXT("layout(binding=")+UTF8String::valueOf(i);        //only debug
                }
                else
                {
                    front=U8_TEXT("layout(set=")+UTF8String::valueOf((int)(*ubo)->set_number)
                         +U8_TEXT(",binding=")  +UTF8String::valueOf((int)(*ubo)->binding);
                }

                if(shader_lib::CheckStruct((*ubo)->type_name))
                {
                    //front+=U8_TEXT(",row_major) uniform");
                    front+=U8_TEXT(",std140) uniform");
                    shader_lib::AddStruct(shader_text,front,(*ubo)->type_name,(*ubo)->value_name);
                }
                else
                {
                    front+=U8_TEXT(") uniform");
                    shader_text.Add(front+u8" "+(*ubo)->type_name+u8" "+(*ubo)->value_name+u8";");
                }

                info_output->colorWriteln("purple",xs->ext_name+UTF8String(": ")+front+UTF8String(" ")+(*ubo)->type_name+U8_TEXT(" ")+(*ubo)->value_name);

                ++ubo;

                if(i<count-1)
                    OutEnter();
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

        ShaderMaker(shader_lib::XMLShader *_xs,MaterialStat *ss,InfoOutput *i_o)
        {
            mtl_stat=ss;
            info_output=i_o;
            xs=_xs;
        }

        ~ShaderMaker()=default;

        bool Make()
        {
            ExpendDepend();

            if(!CheckShader())return(false);
            
            xs->shader_type=glsl_compiler::GetType(xs->ext_name.c_str());
        
            CreateHeader();

            MakeVarying(VaryingType::Input,xs->in);

            MakeUniforms();
            
            MakeRaw();
            MakeModuleCodes();

            MakeVarying(VaryingType::Output,xs->out);

            MakeMainFunc();

            return xs->SetShaderSource(shader_text,info_output);
        }
    };//class ShaderMaker

    bool XMLShaderMaker(shader_lib::XMLShader *xs,MaterialStat *stat,InfoOutput *info_output)
    {
        if(!xs)return(false);

        ShaderMaker sm(xs,stat,info_output);

        /*  example

                filename:   1.vert.xml
                short_name: 1.vert
                ext_name:   vert
                glsl_name:  1.vert.glsl
                spv:        1.vert.spv
                shader:     1.vert.shader
        */

        info_output->colorWriteln("blue","------ Generate "+xs->ext_name+" shader ------");

        return sm.Make();
        
        /*
        {
            const OSString glsl_filename=short_name+OS_TEXT(".glsl");

            xs->SaveToGLSL(glsl_filename);

            if(!result)
            {
                os_err<<OS_TEXT("Error GLSL: ")<<glsl_filename.c_str()<<std::endl;

                return(false);
            }
        }*/
        
        //xs->SaveToSPV(short_name+OS_TEXT(".spv"));
        //xs->SaveToShader(short_name+OS_TEXT(".shader"));

        //return(true);
    }
}//namespace shader_lib