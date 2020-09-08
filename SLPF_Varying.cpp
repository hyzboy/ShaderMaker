#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/filesystem/FileSystem.h>
#include"ShaderLib.h"

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        MapObject<UTF8String,VaryingConfig> varying_config_list;
    //-------------------------------------------------------------------------------
        class VaryingElementCreater:public xml::ElementCreater
        {
            VaryingConfig *vc;
            Varying *vary;

            UTF8String type;
            UTF8String name;
            UTF8String comment;

        public:

            VaryingElementCreater(VaryingConfig *_vc):xml::ElementCreater("varying")
            {
                vc=_vc;
                vary=nullptr;
            }

            bool Init() override
            {
                type.Clear();
                name.Clear();
                comment.Clear();

                vary=new Varying;
                return(true);
            }

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(hgl::stricmp(flag,U8_TEXT("type"))==0)vary->type=info;else
                if(hgl::stricmp(flag,U8_TEXT("name"))==0)vary->name=info;else
                if(hgl::stricmp(flag,U8_TEXT("comment"))==0)vary->comment=info;
            }

            void End()
            {
                std::cout<<"        "<<vary->type.c_str()<<" "<<vary->name.c_str()<<";";
            
                if(!vary->comment.IsEmpty())std::cout<<"    //"<<vary->comment.c_str()<<std::endl;

                vc->Add(vary);
                vary=nullptr;
            }
        };//class VaryingElementCreater:public xml::ElementCreater

        class VaryingRootElementCreater:public xml::ElementCreater
        {
            OSString cur_path;

            VaryingElementCreater *varying;

        public:

            VaryingRootElementCreater(VaryingConfig *vc):xml::ElementCreater("root")
            {
                varying=new VaryingElementCreater(vc);

                Registry(varying);
            }

            virtual ~VaryingRootElementCreater()
            {
                delete varying;
            }
        };//class ShaderLibRootElementCreater:public xml::ElementCreater
    }//namespace

    bool CheckVarying(const UTF8String &name)
    {
        return varying_config_list.KeyExist(name);
    }
        
    bool CheckVarying(const UTF8StringList &vary_list)
    {
        const int count=vary_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
            if(!CheckVarying(vary_list.GetString(i)))return(false);

        return(true);
    }

    VaryingConfig *GetVarying(const UTF8String &name)
    {
        return varying_config_list[name];
    }

    bool LoadVarying(const hgl::OSString &filename)
    {
        VaryingConfig *vc=new VaryingConfig;

        VaryingRootElementCreater root_ec(vc);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      Varying config filename: ")<<filename.c_str()<<std::endl;

        if(!xml::XMLParseFile(&xml,filename))
        {
            delete vc;
            return(false);
        }

        const OSString fn=filesystem::ClipFileMainname(filename);

        varying_config_list.Add(to_u8(fn),vc);
        return(true);
    }
}//namespace shader_lib
