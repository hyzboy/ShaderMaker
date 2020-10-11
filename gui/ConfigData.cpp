#include"ConfigData.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/io/TextOutputStream.h>
#include<QMessageBox>
#include<hgl/util/json/JsonTool.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

namespace
{
    OSString cfg_filename;

    OSString shader_library_path;
    OSString material_source_path;
    OSString material_output_path;
}//namespace

const OSString &GetShaderLibraryPath(){return shader_library_path;}
const OSString &GetMaterialSourcePath(){return material_source_path;}
const OSString &GetMaterialOutputPath(){return material_output_path;}

void SetShaderLibraryPath(const OSString &path){shader_library_path=path;}
void SetMaterialSourcePath(const OSString &path){material_source_path=path;}
void SetMaterialOutputPath(const OSString &path){material_output_path=path;}

bool LoadConfig()
{
    OSString cur_path;

    if(!GetLocalAppdataPath(cur_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","Get path failed which Local AppData",QMessageBox::StandardButton::Abort);

        return(false);
    }

    cfg_filename=MergeFilename(cur_path,OS_TEXT("shader_libs.config"));

    if(!FileExist(cfg_filename))
        return(false);

    {
        Json::Value root;
        OSString error_info;

        if(LoadJson(root,cfg_filename,error_info))
        {
            #define PARSE_JSON_STR(name)    if(root.isMember(#name))name=ToOSString(root[#name].asCString());

            PARSE_JSON_STR(shader_library_path  )
            PARSE_JSON_STR(material_source_path )
            PARSE_JSON_STR(material_output_path )

            #undef PARSE_JSON_STR

            return(true);
        }

        QMessageBox::warning(nullptr,"Fatal Error","Load config file failed.",QMessageBox::StandardButton::Abort);
    }

    return(false);
}

void SaveConfigData()
{
    UTF8StringList sl;

    Json::Value root;

    #define SET_JSON_STR(name)  root[#name]=Json::Value(ToUTF8String(name));

    SET_JSON_STR(shader_library_path)
    SET_JSON_STR(material_source_path)
    SET_JSON_STR(material_output_path)

    #undef SET_JSON_STR
    
    OSString error_info;

    if(!SaveJson(root,cfg_filename,error_info))
    {
        QMessageBox::warning(nullptr,"Fatal Error",QString("can't save config file.\n\nfilename: ")+toQString(cfg_filename),QMessageBox::StandardButton::Abort);
    }
}