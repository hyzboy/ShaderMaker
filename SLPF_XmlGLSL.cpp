#include<hgl/type/StringList.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/filesystem/FileSystem.h>
#include"XMLShader.h"
#include"InfoOutput.h"

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        struct ShaderStruct
        {
            UTF8String cpp;
            UTF8StringList codes;
        };//

        UTF8StringList struct_name_list;
        MapObject<UTF8String,ShaderStruct> struct_list;

        UTF8StringList module_name_list;
        MapObject<UTF8String,XMLShaderModule> xml_module_list;

        class StructElementCreater:public xml::ElementAttribute
        {        
            XMLShaderModule *xsm;
            UTF8String name;
            ShaderStruct *ss;

        public:

            StructElementCreater(XMLShaderModule *sm):ElementAttribute(u8"struct")
            {
                xsm=sm;
                ss=nullptr;
            }
            virtual ~StructElementCreater()=default;

            bool Start() override
            {
                ss=new ShaderStruct;

                name=(*this)[u8"name"];
                ss->cpp=(*this)[u8"cpp"];

                return(true);
            }

            void CharData(const u8char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;

                int len=str_length;
                const u8char *trim_str=hgl::trim(str,len);

                if(trim_str&&len>0)
                    ss->codes.Add(UTF8String(trim_str,len));
            }

            void End() override 
            {
                const int lines=ss->codes.GetCount();

                std::cout<<"        struct "<<name.c_str()<<std::endl
                         <<"        {"<<std::endl;

                for(int i=0;i<lines;i++)
                    std::cout<<"            "<<ss->codes.GetString(i).c_str()<<std::endl;

                std::cout<<"        };"<<std::endl;

                struct_list.Add(name,ss);
                struct_name_list.Add(name);

                xsm->struct_list.Add(name);
                ss=nullptr;
            }
        };//class StructElementCreater:public ElementAttribute

        class DependElementCreater:public xml::ElementCreater
        {
            UTF8StringList *depend_list;

        public:

            DependElementCreater(const u8char *name,UTF8StringList *dl):ElementCreater(name)
            {
                depend_list=dl;
            }

            void CharData(const u8char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;
                    
                int len=str_length;
                const u8char *trim_str=hgl::trim(str,len);

                if(trim_str&&len>0)
                    depend_list->Add(UTF8String(trim_str,len));
            }
        };//class DependElementCreater:public xml::ElementCreater

        class CodeElementCreater:public xml::ElementCreater
        {
            XMLShaderModule *xsm;

        public:

            CodeElementCreater(XMLShaderModule *sm):ElementCreater(u8"code")
            {
                xsm=sm;
            }

            void CharData(const u8char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;
                    
                int len=str_length;
                const u8char *trim_str=hgl::trim(str,len);

                if(trim_str&&len>0)
                    xsm->codes.Add(UTF8String(trim_str,len));
            }
        };//class CodeElementCreater:public xml::ElementCreater

        class RootElementCreater:public xml::ElementCreater
        {
            XMLShaderModule *xsm;
            
            DependElementCreater *depend_raw;
            CodeElementCreater *code;
            StructElementCreater *struct_ec;

        public:

            RootElementCreater(XMLShaderModule *sm):ElementCreater(u8"root")
            {
                xsm=sm;

                depend_raw=new DependElementCreater(u8"raw",&(xsm->depend_raw_list));
                code=new CodeElementCreater(xsm);
                struct_ec=new StructElementCreater(xsm);

                Registry(depend_raw);
                Registry(code);
                Registry(struct_ec);
            }

            virtual ~RootElementCreater()
            {
                delete struct_ec;
                delete code;
                delete depend_raw;
            }
        };//class RootElementCreater:public ElementCreater
    }//namespace

    const UTF8StringList &GetStructList()
    {
        return struct_name_list;
    }

    bool CheckStruct(const UTF8String &name)
    {
        return struct_list.KeyExist(name);
    }

    bool CheckStruct(const UTF8StringList &module_list,InfoOutput *info_output)
    {
        const int count=module_list.GetCount();

        if(count<=0)return(true);

        for(const UTF8String *module_name:module_list)
            if(!CheckStruct(*module_name))
            {                
                if(info_output)
                    info_output->colorWrite("red","can't find Module: "+(*module_name));

                return(false);
            }

        return(true);
    }

    bool AddStruct(UTF8StringList &shader_text,const UTF8String &front,const UTF8String &name,const UTF8String &back)
    {
        ShaderStruct *ss=struct_list[name];

        if(!ss)
            return(false);
            
        const int lines=ss->codes.GetCount();

        if(ss->cpp.IsEmpty())
        shader_text.Add(front+u8" "+name);
        else
        shader_text.Add(front+u8" "+name+u8"    // C++: "+ss->cpp);
        
        shader_text.Add(u8"{");

        for(int i=0;i<lines;i++)
            shader_text.Add(u8"    "+ss->codes.GetString(i));

        if(back.IsEmpty())
            shader_text.Add(u8"};");
        else
            shader_text.Add(u8"}"+back+u8";");

        return(true);
    }

    bool LoadXmlGLSL(const hgl::OSString &filename)
    {
        XMLShaderModule *xsm=new XMLShaderModule;

        RootElementCreater root_ec(xsm);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      XMLGLSL config filename: ")<<filename.c_str()<<std::endl;

        if(!xml::XMLParseFile(&xml,filename))
        {
            delete xsm;
            return(false);
        }

        const OSString module_name=filesystem::ClipFileMainname(filename);
        const UTF8String u8name=to_u8(module_name);

        xml_module_list.Add(u8name,xsm);
        module_name_list.Add(u8name);

        return(true);
    }

    const UTF8StringList &GetModuleList()
    {
        return module_name_list;
    }

    bool CheckModule(const UTF8String &module_name)
    {
        return xml_module_list.KeyExist(module_name);
    }

    bool CheckModules(const UTF8StringList &module_list,InfoOutput *info_output)
    {
        const int count=module_list.GetCount();

        if(count<=0)return(true);

        for(const UTF8String *module_name:module_list)
            if(!CheckModule(*module_name))
            {
                if(info_output)
                    info_output->colorWriteln("red","can't find Module: "+(*module_name));

                return(false);
            }

        return(true);        
    }

    XMLShaderModule *GetShaderModule(const UTF8String &module_name)
    {
        return xml_module_list[module_name];
    }
}//namespace shader_lib
