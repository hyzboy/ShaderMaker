#include"ShaderModule.h"
#include<hgl/util/xml/XMLParse.h>

namespace shader
{
    namespace
    {
        class XMLElementParse
        {
        public:

            virtual ~XMLElementParse()=default;

        public:

            AnsiString  ToAnsiString    (const char *str){return AnsiString(str);}
            UTF8String  ToUTF8String    (const char *str){return UTF8String((u8char *)str);}
            UTF16String ToUTF16String   (const char *str){return to_u16(str);}

        public:

            virtual bool Attr       (const char *flag,const char *info)
            {
                
            }

            virtual bool CharData   (const char *str,int str_length){}
            virtual void End        (){}
        };

        class AdvXMLParse:public XMLParse
        {
            XMLElementParse *element_parse=nullptr;

        public:

            using XMLParse::XMLParse;
            virtual ~AdvXMLParse()=default;            

            virtual XMLElementParse *CreateAttrParse(const char *element_name)=0;

            void StartElement(const char *element_name,const char **atts) override
            {
                element_parse=CreateAttrParse(element_name);

                if(!element_parse)return;

                const char *flag;
                const char *info;

                while(*atts)
                {
                    flag=*atts;++atts;
                    info=*atts;++atts;
                    
                    element_parse->Attr(flag,info);
                }
            }

            void CharData(const char *str,int str_length) override
            {
                if(element_parse)
                    element_parse->CharData(str,str_length);
            }

            void EndElement(const char *element_name) override
            {
                if(!element_parse)return;

                element_parse->End();

                element_parse=nullptr;
            }
        };

        class ShaderModuleXMLParse:public XMLParse
        {
        public:

            void StartElement(const char *element_name,const char **atts) override
            {
            }

            void CharData(const char *str,int str_length) override
            {
            }

            void EndElement(const char *element_name) override
            {
            }
        };//class ShaderModuleXMLParse:public XMLParse
    }//namespace
}//namespace shader
