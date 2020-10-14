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
        UTF8StringList varying_list;
        MapObject<UTF8String,VaryingConfig> varying_config_list;
    //-------------------------------------------------------------------------------
        class VaryingElementCreater:public xml::ElementAttribute
        {
            VaryingConfig *vc;
            Varying *vary;

        public:

            VaryingElementCreater(VaryingConfig *_vc):xml::ElementAttribute("varying")
            {
                vc=_vc;
                vary=nullptr;
            }

            bool Start() override
            {
                vary=new Varying;

                vary->type=(*this)[u8"type"];
                vary->name=(*this)[u8"name"];
                vary->comment=(*this)[u8"comment"];

                return(true);
            }

            void End()
            {
                std::cout<<"        "<<vary->type.c_str()<<" "<<vary->name.c_str()<<";";
            
                if(!vary->comment.IsEmpty())std::cout<<"    //"<<vary->comment.c_str()<<std::endl;

                vc->Add(vary);
                vary=nullptr;
            }
        };//class VaryingElementCreater:public xml::ElementAttribute

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

    const UTF8StringList &GetVaryingList()
    {
        return varying_list;
    }

    bool CheckVarying(const UTF8String &name)
    {
        return varying_config_list.KeyExist(name);
    }
        
    bool CheckVarying(const UTF8StringList &vary_list,InfoOutput *info_output)
    {
        const int count=vary_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
        {
            const auto &vary=vary_list.GetString(i);

            if(!CheckVarying(vary))
            {
                if(info_output)
                    info_output->colorWriteln("red","can't find Varing: "+vary);

                return(false);
            }
        }

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
        const UTF8String u8fn=to_u8(fn);

        varying_config_list.Add(u8fn,vc);
        varying_list.Add(u8fn);

        return(true);
    }
}//namespace shader_lib
