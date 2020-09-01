#include"ShaderLib.h"
#include"ShaderModule.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");

        class FolderElementCreater:public xml::ElementCreater
        {
            enum class FolderType
            {
                Module,
                Varying,
            };//

            OSString path;
            FolderType type;

        public:

            FolderElementCreater():xml::ElementCreater("folder"){}
            virtual ~FolderElementCreater()=default;

            bool Start() override
            {
                path.Clear();
                return(true);
            }

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(stricmp(flag,"path")==0)path=ToOSString(info);else
                if(stricmp(flag,"type")==0)
                {
                    if(stricmp(info,"module")==0)type=FolderType::Module;else
                    if(stricmp(info,"varying")==0)type=FolderType::Varying;else
                    {
                        path.Clear();
                    }
                }
            }

            void End() override
            {
                
            }
        };//class FolderElementCreater:public xml::ElementCreater

        class ShaderLibRootElementCreater:public xml::ElementCreater
        {
            FolderElementCreater *folder;

        public:

            ShaderLibRootElementCreater():xml::ElementCreater("root")
            {
                folder=new FolderElementCreater();
                Registry(folder);
            }

            virtual ~ShaderLibRootElementCreater()
            {
                SAFE_CLEAR(folder);
            }
        };//class ShaderLibRootElementCreater:public xml::ElementCreater
    }//namespace

    bool Init(const OSString &path)
    {
        const OSString filename=filesystem::MergeFilename(path,OSString(shader_libs_filename));

        if(!filesystem::FileExist(filename))
            return(false);

        ShaderLibRootElementCreater root_ec;
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        return xml::XMLParseFile(&xml,filename);
    }

    void Clear()
    {
    }
}//namespace shader_lib
