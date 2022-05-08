#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"
#include"GLSLCompiler.h"

/**
 * 材质文件需要描述的内容
 * 
 * 1.这个材质的类型
 *      1.1.前向型(用于任何直接作用于GBuffer缓冲区的渲染、包括前向渲染和透明物体渲染以及后期)
 *      1.2.延迟几何部分(指向GBuffer提供数据的用户可自定义部分)
 *      1.3.延迟光照合成部分(只有fragment shader)
 *      1.4.UI材质(只输出颜色、不处理深度、法线等等）
 * 2.GBuffer格式与光照模式
 * 
 * 3.使用到的GBuffer缓冲区
 * 
 * 4.使用那几种shader
 * 
 * 5.每种shader之间传递的数据(包括最前端的顶点输入流)
 */

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        class ShaderElementAttribute:public xml::ElementAttribute
        {
            XMLMaterial *xml_material;
            OSString pathname;

            InfoOutput *info_output;

        public:

            ShaderElementAttribute(XMLMaterial *xm,const OSString &pn,InfoOutput *i_o):xml::ElementAttribute("shader")
            {
                xml_material=xm;
                pathname=pn;
                info_output=i_o;
            }

            ~ShaderElementAttribute()=default;

            bool Start()
            {
                const UTF8String st_name=(*this)["type"];
                const UTF8String filename=(*this)["filename"];

                const uint32_t shader_type=glsl_compiler::GetType(st_name);

                const UTF8String xml_fn=filename+u8"."+st_name+u8".xml";

                OSString xml_fullname=filesystem::FixFilename(filesystem::MergeFilename(pathname,ToOSString(xml_fn)));                

                info_output->colorWrite("blue",OS_TEXT("<p>shader: ")+xml_fullname+OS_TEXT("</p>"));

                XMLShader *xs=LoadXMLShader(xml_fullname,info_output);

                if(xs)
                {
                    info_output->colorWrite("green",OS_TEXT("<p>Load XML Shader file \"")+xml_fullname+OS_TEXT("\" OK!</p>"));
                    
                    xml_material->shader_map.Add(shader_type,xs);
                    xml_material->shader_stage_bits|=shader_type;

                    return(true);
                }
                else
                {
                    info_output->colorWrite("red",OS_TEXT("<p>Load XML Shader file \"")+xml_fullname+OS_TEXT("\" failed!</p>"));
                
                    return(false);
                }
            }
        };//class ShaderElementAttribute:public xml::ElementAttribute

        class XMLMaterialRootElementCreater:public xml::ElementCreater
        {
            OSString filename;
            OSString pathname;

            XMLMaterial *xml_material;

            ShaderElementAttribute *shader;

        public:

            XMLMaterialRootElementCreater(const OSString &fn,XMLMaterial *xm,InfoOutput *i_o):xml::ElementCreater("root")
            {
                filename=fn;
                xml_material=xm;

                pathname=filesystem::ClipPathname(filename);

                shader=new ShaderElementAttribute(xml_material,pathname,i_o);

                Registry(shader);
            }

            ~XMLMaterialRootElementCreater()
            {
                delete shader;
            }
        };//class XMLMaterialRootElementCreater:public xml::ElementCreater
    }//namespace
    
    void DescSetUniformList::Add(const uint32 shader_type,Uniform *u)
    {
        MaterialUniform *mu;
        
        if(mu_list.Get(u->value_name,mu))
        {
            mu->shader_stage_flag|=shader_type;
        }
        else
        {
            mu=new MaterialUniform(shader_type,u);

            mu_list.Add(u->value_name,mu);
        }

        uniform_list.Add(u);
    }

    void DescSetUniformList::RefreshDescSetBinding(const int sn)
    {
        set_number=sn;

        for(Uniform *u:uniform_list)
        {
            u->set_number=sn;
            u->binding=mu_list.Find(u->value_name);
        }
    }

    XMLMaterial *LoadXMLMaterial(const OSString &filename,InfoOutput *info_output)
    {
        if(!filesystem::FileExist(filename))
        {
            if(info_output)
            {
                OSString info=OS_TEXT("<p>Can't find \"")+filename+OS_TEXT("\"</p>");

                info_output->colorWrite("red",info.c_str());
            }

            return(nullptr);
        }

        XMLMaterial *xml_material=new XMLMaterial;

        XMLMaterialRootElementCreater root_ec(filename,xml_material,info_output);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        if(!xml::XMLParseFile(&xml,filename))
        {
            delete xml_material;
            return(nullptr);
        }

        {
            {
                bool set_has[(size_t)DescriptorSetsType::RANGE_SIZE];

                hgl_zero(set_has);

                for(int i=0;i<xml_material->shader_map.GetCount();i++)
                {
                    XMLShader *xs;

                    xml_material->shader_map.GetValue(i,xs);

                    for(auto it:xs->uniforms)
                    {
                        set_has[(size_t)it->type]=true;

                        xml_material->mtl_stat.ds_uniform[(size_t)it->type].Add(xs->shader_type,it);
                    }
                }

                {
                    uint32_t index=0;

                    ENUM_CLASS_FOR(DescriptorSetsType,int,i)
                    {
                        if(set_has[i])
                        {
                            xml_material->mtl_stat.ds_uniform[i].RefreshDescSetBinding(index);
                            ++index;
                        }
                    }
                }
            }

            for(int i=0;i<xml_material->shader_map.GetCount();i++)
            {
                XMLShader *xs;

                xml_material->shader_map.GetValue(i,xs);

                if(XMLShaderMaker(xs,&(xml_material->mtl_stat),info_output))
                {
                    info_output->colorWrite("green",OS_TEXT("<p>Make shader from \"")+xs->origin_filename+OS_TEXT("\" OK!</p>"));
                }
                else
                {
                    info_output->colorWrite("red",OS_TEXT("<p>Make shader from \"")+xs->origin_filename+OS_TEXT("\" failed!</p>"));
                    delete xml_material;
                    return(nullptr);
                }
            }
        }
        
        if(info_output)
        {
            OSString info=OS_TEXT("<p>Load Material file \"")+filename+OS_TEXT("\" OK!</p>");
            info_output->colorWrite("green",info.c_str());
        }

        return xml_material;
    }
}//namespace shader_lib
