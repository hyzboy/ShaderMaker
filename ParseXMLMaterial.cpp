#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"
#include"GLSLCompiler.h"

namespace shader_lib
{
    using namespace hgl;
    
    XMLShader *LoadXMLShader(const OSString &filename);
    bool XMLShaderMaker(const OSString &filename,XMLShader *xs);

    namespace
    {
        class ShaderElementAttribute:public xml::ElementAttribute
        {
            XMLMaterial *xml_material;
            OSString pathname;

        public:

            ShaderElementAttribute(XMLMaterial *xm,const OSString &pn):xml::ElementAttribute("shader")
            {
                xml_material=xm;
                pathname=pn;
            }

            ~ShaderElementAttribute()=default;

            bool Start()
            {
                const UTF8String st_name=(*this)["type"];
                const UTF8String filename=(*this)["filename"];

                const uint32_t shader_type=glsl_compiler::GetType(st_name);

                const UTF8String xml_fn=filename+u8"."+st_name+u8".xml";

                const OSString xml_fullname=filesystem::MergeFilename(pathname,ToOSString(xml_fn));

                os_out<<"shader: "<<xml_fullname.c_str()<<std::endl;

                XMLShader *xs=LoadXMLShader(xml_fullname);

                if(xs)
                {
                    os_out<<OS_TEXT("Load XML Shader file<")<<xml_fullname.c_str()<<OS_TEXT("> ok!")<<std::endl;

                    xml_material->shaders.Add(shader_type,xs);
                    xml_material->shader_bits|=shader_type;

                    if(XMLShaderMaker(xml_fullname,xs))
                    {
                        os_out<<OS_TEXT("Make Shader from <")<<xml_fullname.c_str()<<OS_TEXT("> ok!")<<std::endl;

                        return(true);
                    }
                    else
                    {
                        os_out<<OS_TEXT("Make Shader from <")<<xml_fullname.c_str()<<OS_TEXT("> failed!")<<std::endl;
                        return(false);
                    }
                }
                else
                {
                    os_out<<OS_TEXT("Load XML Shader file<")<<xml_fullname.c_str()<<OS_TEXT("> error!")<<std::endl;
                
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

            XMLMaterialRootElementCreater(const OSString &fn,XMLMaterial *xm):xml::ElementCreater("root")
            {
                filename=fn;
                xml_material=xm;

                pathname=filesystem::ClipPathname(filename);

                shader=new ShaderElementAttribute(xml_material,pathname);

                Registry(shader);
            }

            ~XMLMaterialRootElementCreater()
            {
                delete shader;
            }
        };//class XMLMaterialRootElementCreater:public xml::ElementCreater
    }//namespace

    XMLMaterial *LoadXMLMaterial(const OSString &filename)
    {
        if(!filesystem::FileExist(filename))
        {
            os_out<<OS_TEXT("filename <")<<filename.c_str()<<OS_TEXT("> don't exist!")<<std::endl;
            return(nullptr);
        }

        XMLMaterial *xml_material=new XMLMaterial;

        XMLMaterialRootElementCreater root_ec(filename,xml_material);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        if(!xml::XMLParseFile(&xml,filename))
        {
            delete xml_material;
            return(nullptr);
        }
        
        os_out<<OS_TEXT("Load XML Material file<")<<filename.c_str()<<OS_TEXT("> ok!")<<std::endl;

        return xml_material;
    }
}//namespace shader_lib