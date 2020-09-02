#ifndef HGL_SHADER_LIB_PARSE_INCLUDE
#define HGL_SHADER_LIB_PARSE_INCLUDE

#include<hgl/util/xml/ElementParseCreater.h>

namespace shader_lib
{
    using namespace hgl;

    class FolderElementCreater:public xml::ElementCreater
    {
        enum class FolderType
        {
            Module=0,
            Varying,
        };//

        OSString uplevel_path;

        OSString path;
        FolderType type;

    public:

        FolderElementCreater(const OSString &ulp):xml::ElementCreater("folder")
        {
            uplevel_path=ulp;
        }
        virtual ~FolderElementCreater()=default;

        bool Start() override;
        void Attr(const u8char *flag,const u8char *info) override;
        void End() override;
    };//class FolderElementCreater:public xml::ElementCreater

    class FilesElementerCreater:public xml::ElementCreater
    {
    public:

        FilesElementerCreater():xml::ElementCreater("files"){}
        virtual ~FilesElementerCreater()=default;
    
        void CharData(const u8char *str,const int str_length) override;
    };//class FilesElementerCreater:public xml::ElementCreater

    class ShaderLibRootElementCreater:public xml::ElementCreater
    {
        OSString cur_path;

        FolderElementCreater *folder;
        FilesElementerCreater *files;

    public:

        ShaderLibRootElementCreater(const OSString &);
        virtual ~ShaderLibRootElementCreater();
    };//class ShaderLibRootElementCreater:public xml::ElementCreater
}//namespace shader_lib
#endif//HGL_SHADER_LIB_PARSE_INCLUDE