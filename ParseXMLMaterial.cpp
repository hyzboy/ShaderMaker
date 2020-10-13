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

                os_out<<"shader: "<<xml_fullname.c_str()<<std::endl;

                XMLShader *xs=LoadXMLShader(xml_fullname,info_output);

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

    bool SaveMaterial(const OSString &filename,XMLMaterial *xm)
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
            os_out<<OS_TEXT("Save Material file<")<<filename.c_str()<<OS_TEXT("> ok!")<<std::endl;
            return(true);
        }
        else
        {
            os_out<<OS_TEXT("Convert Material file failed!")<<std::endl;
            return(false);
        }
    }

    XMLMaterial *LoadXMLMaterial(const OSString &filename,InfoOutput *info_output)
    {
        if(!filesystem::FileExist(filename))
        {
            os_out<<OS_TEXT("filename <")<<filename.c_str()<<OS_TEXT("> don't exist!")<<std::endl;
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
        
        os_out<<OS_TEXT("Load XML Material file<")<<filename.c_str()<<OS_TEXT("> ok!")<<std::endl;

        const OSString mat_filename=filesystem::TrimFileExtName(filename,true);

        SaveMaterial(mat_filename,xml_material);

        return xml_material;
    }
}//namespace shader_lib
