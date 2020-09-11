#ifndef HGL_SHADER_LIB_PARSE_INCLUDE
#define HGL_SHADER_LIB_PARSE_INCLUDE

#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/StringList.h>

namespace shader_lib
{
    using namespace hgl;

    class FolderElementCreater:public xml::ElementAttribute
    {
        OSString uplevel_path;

        OSString path;

    public:

        FolderElementCreater(const OSString &ulp):xml::ElementAttribute("folder")
        {
            uplevel_path=ulp;
        }
        virtual ~FolderElementCreater()=default;
        
        void CharData(const u8char *str,const int str_length) override;
    };//class FolderElementCreater:public xml::ElementAttribute

    class FilesElementerCreater:public xml::ElementAttribute
    {
        enum class FileType
        {
            GLSL=0,
            Varying,

            ENUM_CLASS_RANGE(GLSL,Varying)
        };

        OSString pathname;

        FileType type;

        OSStringList file_list;

    public:

        FilesElementerCreater(const OSString &path):xml::ElementAttribute("files")
        {
            pathname=path;
            type=FileType::GLSL;
        }
        virtual ~FilesElementerCreater()=default;
    
        bool Start() override;
        void CharData(const u8char *str,const int str_length) override;
    };//class FilesElementerCreater:public xml::ElementAttribute

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