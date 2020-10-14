#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>
#include<hgl/io/MemoryOutputStream.h>
#include"ShaderLib.h"
#include"GLSLCompiler.h"

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

                const OSString xml_fullname=filesystem::MergeFilename(pathname,ToOSString(xml_fn));

                info_output->colorWrite("blue",OS_TEXT("<p>shader: ")+xml_fullname+OS_TEXT("</p>"));

                XMLShader *xs=LoadXMLShader(xml_fullname,info_output);

                if(xs)
                {
                    info_output->colorWrite("green",OS_TEXT("<p>Load XML Shader file \"")+xml_fullname+OS_TEXT("\" OK!</p>"));

                    xml_material->shaders.Add(shader_type,xs);
                    xml_material->shader_bits|=shader_type;

                    if(XMLShaderMaker(xs,info_output))
                    {
                        info_output->colorWrite("green",OS_TEXT("<p>Make shader from \"")+xml_fullname+OS_TEXT("\" OK!</p>"));

                        return(true);
                    }
                    else
                    {
                        info_output->colorWrite("red",OS_TEXT("<p>Make shader from \"")+xml_fullname+OS_TEXT("\" failed!</p>"));
                        return(false);
                    }
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

    void SaveSPV(io::DataOutputStream *global_dos,const uint32_t flag,glsl_compiler::SPVData *spv)
    {
        io::MemoryOutputStream mos;

        glsl_compiler::SaveSPV2Shader(&mos,spv,flag,false);

        global_dos->WriteUint32(mos.Tell());
        global_dos->Write(mos.GetData(),mos.Tell());
    }

    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output)
    {
        if(!xm)return(false);

        constexpr char MaterialFileHeader[]=u8"Material\x1A";
        constexpr uint MaterialFileHeaderLength=sizeof(MaterialFileHeader)-1;

        io::MemoryOutputStream mos;
        io::LEDataOutputStream dos(&mos);

        dos.Write(MaterialFileHeader,MaterialFileHeaderLength);
        dos.WriteUint8(1);                                                      //version

        dos.WriteUint32(xm->shader_bits);

        const uint count=xm->shaders.GetCount();
        auto **sp=xm->shaders.GetDataList();
        for(uint i=0;i<count;i++)
        {
            SaveSPV(&dos,
                    (*sp)->left,
                    (*sp)->right->spv_data);

            ++sp;
        }

        if(filesystem::SaveMemoryToFile(filename,mos.GetData(),mos.Tell())==mos.Tell())
        {
            info_output->colorWriteln("green",OS_TEXT("Save material file \"<b>")+filename+OS_TEXT("</b>\" OK! total ")+OSString::valueOf(mos.Tell())+OS_TEXT(" bytes."));
            return(true);
        }
        else
        {
            info_output->colorWriteln("red",OS_TEXT("Save material file \"")+filename+OS_TEXT("\" failed!"));
            return(false);
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
        
        if(info_output)
        {
            OSString info=OS_TEXT("<p>Load Material file \"")+filename+OS_TEXT("\" OK!</p>");
            info_output->colorWrite("green",info.c_str());
        }

        return xml_material;
    }
}//namespace shader_lib
