#include<hgl/type/StringList.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/filesystem/FileSystem.h>

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

        MapObject<UTF8String,ShaderStruct> struct_list;   
                
        class StructElementCreater:public xml::ElementAttribute
        {
            UTF8String name;
            ShaderStruct *ss;

        public:

            StructElementCreater():ElementAttribute(u8"struct"){ss=nullptr;}
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
                ss=nullptr;
            }
        };//class StructElementCreater:public ElementAttribute

        class RootElementCreater:public xml::ElementCreater
        {        
            StructElementCreater struct_ec;

        public:

            RootElementCreater():ElementCreater(u8"root")
            {
                Registry(&struct_ec);
            }

            virtual ~RootElementCreater()=default;
        };//class RootElementCreater:public ElementCreater
    }//namespace

    bool CheckStruct(const UTF8String &name)
    {
        return struct_list.KeyExist(name);
    }

    bool CheckStruct(const UTF8StringList &module_list)
    {
        const int count=module_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
            if(!CheckStruct(module_list.GetString(i)))return(false);

        return(true);
    }

    bool AddStruct(UTF8StringList &shader_text,const UTF8String &name)
    {
        ShaderStruct *ss=struct_list[name];

        if(!ss)
            return(false);
            
        const int lines=ss->codes.GetCount();

        if(ss->cpp.IsEmpty())
        shader_text.Add(u8"struct "+name);
        else
        shader_text.Add(u8"struct "+name+u8"    // C++: "+ss->cpp);
        
        shader_text.Add(u8"{");

        for(int i=0;i<lines;i++)
            shader_text.Add(u8"    "+ss->codes.GetString(i));

        shader_text.Add(u8"};");
    }

    bool LoadXmlGLSL(const hgl::OSString &filename)
    {
        RootElementCreater root_ec;
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      XMLGLSL config filename: ")<<filename.c_str()<<std::endl;

        return xml::XMLParseFile(&xml,filename);
    }
}//namespace shader_lib
