#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        class FileListElementCreater:public xml::ElementCreater
        {
            UTF8StringList file_list;

        public:

            FileListElementCreater(const u8char *str):xml::ElementCreater(str){}
            virtual ~FileListElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(len>0)
                    file_list.Add(UTF8String(trim_str,len));
            }
        };//class FileListElementCreater:public xml::ElementCreater
        
        class CodesElementCreater:public xml::ElementCreater
        {
            UTF8StringList codes;

        public:

            CodesElementCreater(const u8char *str):xml::ElementCreater(str){}
            virtual ~CodesElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(len>0)
                    codes.Add(UTF8String(trim_str,len));
            }
        };//class CodesElementCreater:public xml::ElementCreater

        class XMLShaderRootElementCreater:public xml::ElementCreater
        {
            OSString filename;

            FileListElementCreater *in,*out,*mdl;
            CodesElementCreater *codes_main;

        public:

            XMLShaderRootElementCreater(const OSString &fn):xml::ElementCreater("root")
            {
                filename=fn;

                in=new FileListElementCreater(u8"in");
                out=new FileListElementCreater(u8"out");
                mdl=new FileListElementCreater(u8"module");
                codes_main=new CodesElementCreater(u8"main");

                Registry(in);
                Registry(out);
                Registry(mdl);
                Registry(codes_main);
            }

            virtual ~XMLShaderRootElementCreater()
            {
                delete codes_main;
                delete mdl;
                delete out;
                delete in;
            }
        };//class XMLShaderRootElementCreater:public xml::ElementCreater
    }//namespace

    bool LoadXMLShader(const OSString &filename)
    {
        if(!filesystem::FileExist(filename))
            return(false);

        XMLShaderRootElementCreater root_ec(filename);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        return xml::XMLParseFile(&xml,filename);        
    }
}//namespace shader_lib
