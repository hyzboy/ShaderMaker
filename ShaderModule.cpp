#include"ShaderModule.h"
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/Stack.h>

namespace shader
{
    namespace
    {
        using namespace hgl::xml;

        class AuthorElementCreater:public ElementCreater
        {
        public:

            AuthorElementCreater():ElementCreater("author"){}
            virtual ~AuthorElementCreater()=default;
        };//class AuthorElementCreater:public ElementCreater

        class TimeElementCreater:public ElementCreater
        {
        public:

            TimeElementCreater():ElementCreater("time"){}
            virtual ~TimeElementCreater()=default;
        };//class TimeElementCreater:public ElementCreater

        class CPPElementCreater:public ElementCreater
        {
            UBO *ubo;

        public:

            CPPElementCreater():ElementCreater("cpp"){ubo=nullptr;}
            virtual ~CPPElementCreater()=default;

            void SetUBO(UBO *su){ubo=su;}
            void End() override {ubo=nullptr;}

            void CharData(const char *str,const int str_length) override
            {
                if(!ubo)return;

                ubo->cpp_files.Add(UTF8String((u8char *)str,str_length));
            }
        };//class CPPElementCreater:public ElementCreater
        
        class UBOCodeElementCreater:public ElementCreater
        {
            UBO *ubo;

        public:

            UBOCodeElementCreater():ElementCreater("code"){ubo=nullptr;}
            virtual ~UBOCodeElementCreater()=default;
            
            void SetUBO(UBO *su){ubo=su;}
            void End() override 
            {
                const int lines=ubo->codes.GetCount();

                std::cout<<"uniform"<<std::endl<<"{"<<std::endl;

                for(int i=0;i<lines;i++)
                    std::cout<<"    "<<ubo->codes.GetString(i).c_str()<<std::endl;

                std::cout<<"};"<<std::endl;

                ubo=nullptr;
            }

            void CharData(const char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;

                if(ubo)
                {
                    UTF8String u8str;
                    LoadStringFromText(u8str,(void *)str,str_length,UTF8CharSet);

                    u8str.Trim();

                    if(!u8str.IsEmpty())
                        ubo->codes.Add(u8str);
                }
            }
        };//class UBOCodeElementCreater:public ElementCreater

        class UBOElementCreater:public ElementCreater
        {
            CPPElementCreater cpp_ec;
            UBOCodeElementCreater code_ec;

            Module *shader_module;
            UBO *ubo;

        public:

            UBOElementCreater(Module *sm):ElementCreater("ubo")
            {
                shader_module=sm;
                ubo=nullptr;

                Registry("cpp",&cpp_ec);
                Registry("code",&code_ec);
            }

            virtual ~UBOElementCreater()=default;

            bool Start() override
            {
                ubo=new UBO;

                cpp_ec.SetUBO(ubo);
                code_ec.SetUBO(ubo);

                return(true);
            }

            void Attr(const char *flag,const char *info) override
            {
                if(strcmp(flag,"name")==0)ubo->name=info;else
                if(strcmp(flag,"value")==0)ubo->value_name=info;                
            }

            void End() override
            {
                if(ubo)
                {
                    shader_module->ubo_list.Add(ubo);
                    ubo=nullptr;
                }
            }
        };//class UBOElementCreater:public ElementCreater

        class ShaderRootElementCreater:public ElementCreater
        {        
            AuthorElementCreater author_ec;
            TimeElementCreater time_ec;
            UBOElementCreater *ubo_ec;

            Module *shader_module;

        public:

            ShaderRootElementCreater(Module *sm):ElementCreater("root")
            {
                shader_module=sm;

                ubo_ec=new UBOElementCreater(sm);

                //Registry("author",&author_ec);
                //Registry("time",&time_ec);
                Registry("ubo",ubo_ec);
            }

            virtual ~ShaderRootElementCreater()
            {
                SAFE_CLEAR(ubo_ec);
            }

            void End() override
            {
            }
        };//class ShaderRootElementCreater:public ElementCreater
    }//namespace

    Module *LoadXMLShader(const OSString &filename)
    {
        Module *sm=new Module;

        sm->filename=filename;

        ShaderRootElementCreater root_ec(sm);
        ElementParseCreater epc(&root_ec);
        XMLParse xp(&epc);

        xp.Start();

        if(!XMLParseFile(&xp,filename))
        {
            delete sm;
            return(nullptr);
        }

        return sm;
    }
}//namespace shader
