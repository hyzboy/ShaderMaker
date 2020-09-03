#ifndef HGL_SHADER_LIB_PARSE_INCLUDE
#define HGL_SHADER_LIB_PARSE_INCLUDE

#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>

namespace shader_lib
{
    using namespace hgl;

    class FolderElementCreater:public xml::ElementCreater
    {
        OSString uplevel_path;

        OSString path;

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
        enum class FileType
        {
            GLSL=0,
            Varying,
        };

        FileType type;

        OSStringList file_list;

    public:

        FilesElementerCreater():xml::ElementCreater("files")
        {
            type=FileType::GLSL;
        }
        virtual ~FilesElementerCreater()=default;
    
        bool Start() override;
        void Attr(const u8char *flag,const u8char *info) override;
        void CharData(const u8char *str,const int str_length) override;
        void End() override;
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