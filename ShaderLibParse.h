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
            Module,
            Varying,
        };//

        OSString path;
        FolderType type;

    public:

        FolderElementCreater():xml::ElementCreater("folder"){}
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
        FolderElementCreater *folder;
        FilesElementerCreater *files;

    public:

        ShaderLibRootElementCreater();
        virtual ~ShaderLibRootElementCreater();
    };//class ShaderLibRootElementCreater:public xml::ElementCreater
}//namespace shader_lib
#endif//HGL_SHADER_LIB_PARSE_INCLUDE