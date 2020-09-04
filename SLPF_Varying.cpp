#include<hgl/type/String.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        class VaryingElementCreater:public xml::ElementCreater
        {
            UTF8String type;
            UTF8String name;
            UTF8String comment;

        public:

            VaryingElementCreater():xml::ElementCreater("varying"){}

            bool Init() override
            {
                type.Clear();
                name.Clear();
                comment.Clear();
                return(true);
            }

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(hgl::stricmp(flag,U8_TEXT("type"))==0)type=info;else
                if(hgl::stricmp(flag,U8_TEXT("name"))==0)name=info;else
                if(hgl::stricmp(flag,U8_TEXT("comment"))==0)comment=info;
            }

            void End()
            {
                std::cout<<"        "<<type.c_str()<<" "<<name.c_str()<<";";
            
                if(!comment.IsEmpty())std::cout<<"    //"<<comment.c_str()<<std::endl;
            }
        };//class VaryingElementCreater:public xml::ElementCreater

        class VaryingRootElementCreater:public xml::ElementCreater
        {
            OSString cur_path;

            VaryingElementCreater *varying;

        public:

            VaryingRootElementCreater():xml::ElementCreater("root")
            {
                varying=new VaryingElementCreater;

                Registry(varying);
            }

            virtual ~VaryingRootElementCreater()
            {
                delete varying;
            }
        };//class ShaderLibRootElementCreater:public xml::ElementCreater
    }//namespace

    bool LoadVarying(const hgl::OSString &filename)
    {
        VaryingRootElementCreater root_ec;
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      Varying config filename: ")<<filename.c_str()<<std::endl;

        return xml::XMLParseFile(&xml,filename);
    }
}//namespace shader_lib
