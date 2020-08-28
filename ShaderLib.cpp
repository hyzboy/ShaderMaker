#include"ShaderLib.h"
#include"ShaderModule.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");

        class ShaderLibsElementParse:public xml::ElementParse
        {
        public:

            using xml::ElementParse::ElementParse;
            virtual ~ShaderLibsElementParse()=default;

            virtual bool Start      (const u8char *element_name)            {return(true);}
            virtual void Attr       (const u8char *flag,const u8char *info) {}
            virtual void CharData   (const u8char *str,const int str_length){}
            virtual void End        (const u8char *element_name)            {}
        };//
    }//namespace

    bool Init(const OSString &path)
    {
        const OSString filename=filesystem::MergeFilename(path,OSString(shader_libs_filename));

        if(!filesystem::FileExist(filename))
            return(false);


    }

    void Clear()
    {
    }
}//namespace shader_lib
