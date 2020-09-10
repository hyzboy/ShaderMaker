#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        class CodesElementCreater:public xml::ElementCreater
        {
            UTF8StringList *codes;

        public:

            CodesElementCreater(const u8char *str,UTF8StringList *c):xml::ElementCreater(str)
            {
                codes=c;
            }
            virtual ~CodesElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(trim_str)
                    codes->Add(UTF8String(trim_str,len));
            }
        };//class CodesElementCreater:public xml::ElementCreater
        
        class UniformElementCreater:public xml::ElementCreater
        {
            XMLShader *xml_shader;

        public:

            UniformElementCreater(XMLShader *xs):xml::ElementCreater(u8"uniform"){xml_shader=xs;}
            virtual ~UniformElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len,notcodechar);

                if(len<=0)return;

                int ll=len;
                int rl=len;
                const u8char *left_str=clipleft(trim_str,ll,notcodechar);
                const u8char *right_str=clipright(trim_str,rl,notcodechar);

                if(left_str&&right_str)
                {
                    Uniform *ubo=new Uniform;

                    ubo->type_name=UTF8String(left_str,ll);
                    ubo->value_name =UTF8String(right_str,rl);

                    if(xml_shader->struct_block.Find(ubo->type_name)==-1)
                        xml_shader->struct_block.Add(ubo->type_name);

                    xml_shader->uniforms.Add(ubo);
                }
            }
        };//class UniformElementCreater:public xml::ElementCreater

        class XMLShaderRootElementCreater:public xml::ElementCreater
        {
            OSString filename;

            XMLShader *xml_shader;

            CodesElementCreater *in,*out,*raw,*codes_main;
            UniformElementCreater *uniform;

        public:

            XMLShaderRootElementCreater(const OSString &fn,XMLShader *xs):xml::ElementCreater("root")
            {
                filename=fn;
                xml_shader=xs;

                in=new CodesElementCreater(u8"in",&(xml_shader->in));
                out=new CodesElementCreater(u8"out",&(xml_shader->out));
                raw=new CodesElementCreater(u8"raw",&(xml_shader->raw));
                codes_main=new CodesElementCreater(u8"main",&(xml_shader->main));

                uniform=new UniformElementCreater(xml_shader);

                Registry(in);
                Registry(out);
                Registry(raw);
                Registry(codes_main);

                Registry(uniform);
            }

            virtual ~XMLShaderRootElementCreater()
            {
                delete uniform;

                delete codes_main;
                delete raw;
                delete out;
                delete in;
            }
        };//class XMLShaderRootElementCreater:public xml::ElementCreater
    }//namespace

    XMLShader *LoadXMLShader(const OSString &filename)
    {
        if(!filesystem::FileExist(filename))
            return(nullptr);

        XMLShader *xml_shader=new XMLShader;

        XMLShaderRootElementCreater root_ec(filename,xml_shader);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        if(!xml::XMLParseFile(&xml,filename))
        {
            delete xml_shader;
            return(nullptr);
        }

        return xml_shader;
    }
}//namespace shader_lib
