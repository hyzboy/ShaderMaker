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

            CodesElementCreater(const u8char *str,UTF8StringList *c):xml::ElementCreater(str){codes=c;}
            virtual ~CodesElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(len>0)
                    codes->Add(UTF8String(trim_str,len));
            }
        };//class CodesElementCreater:public xml::ElementCreater

        class XMLShaderRootElementCreater:public xml::ElementCreater
        {
            OSString filename;

            XMLShader *xml_shader;

            CodesElementCreater *in,*out,*mdl,*raw,*codes_main;

        public:

            XMLShaderRootElementCreater(const OSString &fn,XMLShader *xs):xml::ElementCreater("root")
            {
                filename=fn;
                xml_shader=xs;

                in=new CodesElementCreater(u8"in",&(xml_shader->in));
                out=new CodesElementCreater(u8"out",&(xml_shader->out));
                mdl=new CodesElementCreater(u8"module",&(xml_shader->modules));
                raw=new CodesElementCreater(u8"raw",&(xml_shader->raw));
                codes_main=new CodesElementCreater(u8"main",&(xml_shader->main));

                Registry(in);
                Registry(out);
                Registry(mdl);
                Registry(raw);
                Registry(codes_main);
            }

            virtual ~XMLShaderRootElementCreater()
            {
                delete codes_main;
                delete raw;
                delete mdl;
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
