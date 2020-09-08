#include"ShaderModule.h"
#include<hgl/type/String.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        MapObject<UTF8String,Module> module_list;   

        class CPPElementCreater:public xml::ElementCreater
        {
            SubModule *sm;

        public:

            CPPElementCreater():ElementCreater(u8"cpp"){sm=nullptr;}
            virtual ~CPPElementCreater()=default;

            void Set(SubModule *_sm){sm=_sm;}
            void End() override {sm=nullptr;}

            void CharData(const u8char *str,const int str_length) override
            {
                if(!sm)return;

                int len=str_length;
                const u8char *trim_str=hgl::trim(str,len);

                if(trim_str&&len>0)
                    sm->cpp_files.Add(UTF8String(trim_str,len));
            }
        };//class CPPElementCreater:public ElementCreater
        
        class UBOCodeElementCreater:public xml::ElementCreater
        {
            UBO *ubo;

        public:

            UBOCodeElementCreater():ElementCreater(u8"code"){ubo=nullptr;}
            virtual ~UBOCodeElementCreater()=default;
            
            void SetUBO(UBO *su){ubo=su;}
            void End() override 
            {
                const int lines=ubo->codes.GetCount();

                std::cout<<"        uniform "<<ubo->name.c_str()<<std::endl
                         <<"        {"<<std::endl;

                for(int i=0;i<lines;i++)
                    std::cout<<"            "<<ubo->codes.GetString(i).c_str()<<std::endl;

                std::cout<<"        }"<<ubo->value_name.c_str()<<";"<<std::endl;

                ubo=nullptr;
            }

            void CharData(const u8char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;

                if(ubo)
                {
                    int len=str_length;
                    const u8char *trim_str=hgl::trim(str,len);

                    if(trim_str&&len>0)
                        ubo->codes.Add(UTF8String(trim_str,len));
                }
            }
        };//class UBOCodeElementCreater:public ElementCreater

        class UBOElementCreater:public xml::ElementCreater
        {
            CPPElementCreater cpp_ec;
            UBOCodeElementCreater code_ec;

            Module *shader_module;
            UBO *ubo;

        public:

            UBOElementCreater(Module *sm):ElementCreater(u8"ubo")
            {
                shader_module=sm;
                ubo=nullptr;

                Registry(&cpp_ec);
                Registry(&code_ec);
            }

            virtual ~UBOElementCreater()=default;

            bool Init() override
            {
                ubo=new UBO;

                cpp_ec.Set(ubo);
                code_ec.SetUBO(ubo);

                return(true);
            }

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(!ubo)return;

                if(strcmp(flag,u8"name")==0)ubo->name=info;else
                if(strcmp(flag,u8"value")==0)ubo->value_name=info;                
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

        class RootElementCreater:public xml::ElementCreater
        {        
            UBOElementCreater *ubo_ec;

            Module *shader_module;

        public:

            RootElementCreater(Module *sm):ElementCreater(u8"root")
            {
                shader_module=sm;

                ubo_ec=new UBOElementCreater(sm);

                Registry(ubo_ec);
            }

            virtual ~RootElementCreater()
            {
                SAFE_CLEAR(ubo_ec);
            }
        };//class RootElementCreater:public ElementCreater
    }//namespace

    bool CheckXmlModule(const UTF8String &name)
    {
        return module_list.KeyExist(name);
    }

    bool CheckXmlModule(const UTF8StringList &module_list)
    {
        const int count=module_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
            if(!CheckXmlModule(module_list.GetString(i)))return(false);

        return(true);
    }

    Module *GetXmlModule(const UTF8String &name)
    {
        return module_list[name];
    }

    bool LoadXmlGLSL(const hgl::OSString &filename)
    {
        Module *sm=new Module;

        sm->filename=filename;

        RootElementCreater root_ec(sm);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      XMLGLSL config filename: ")<<filename.c_str()<<std::endl;

        const OSString mn=filesystem::ClipFileMainname(filename);

        module_list.Add(to_u8(mn),sm);

        return xml::XMLParseFile(&xml,filename);
    }
}//namespace shader_lib
