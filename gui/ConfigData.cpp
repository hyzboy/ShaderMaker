#include"ConfigData.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/type/StdString.h>
#include<hgl/io/TextOutputStream.h>
#include<QMessageBox>
#include<hgl/util/json/JsonTool.h>
#include<hgl/qt/Style.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

namespace
{
    OSString cfg_filename;

    struct
    {
        OSString library;
        OSString source;
        OSString output;

        OSString varying;
    }path_config;

    QFont ui_fnt,code_fnt;
}//namespace

const OSString &GetShaderLibraryPath (){return path_config.library;}
const OSString &GetMaterialSourcePath(){return path_config.source;}
const OSString &GetMaterialOutputPath(){return path_config.output;}
const OSString &GetVaryingLibraryPath()
{
    if(path_config.varying.IsEmpty())        
        path_config.varying=filesystem::MergeFilename(path_config.library,OS_TEXT("varying"));

    return path_config.varying;
}

const QFont &GetUIFont()
{
    return ui_fnt;
}

const QFont &GetCodeFont()
{
    return code_fnt;
}

void SetShaderLibraryPath (const OSString &path)
{
    path_config.library=FixFilename(path);

    path_config.varying.Clear();
}

void SetMaterialSourcePath(const OSString &path){path_config.source =FixFilename(path);}
void SetMaterialOutputPath(const OSString &path){path_config.output =FixFilename(path);}

void SetUIFont(const QFont &fnt)
{
    ui_fnt=fnt;
}

void SetCodeFont(const QFont &fnt)
{
    code_fnt=fnt;
}

void LoadFontJson(QFont &fnt,const Json::Value &font_root)
{
    if(!font_root.isMember("name"))
        return;

    if(!font_root.isMember("size"))
        return;

    const std::string fn=font_root["name"].asString();
    const int fs=font_root["size"].asInt();

    fnt.setFamily(QString::fromStdString(fn));
    fnt.setPointSize(fs);
}

bool LoadConfig()
{
    OSString cur_path;

    if(!GetLocalAppdataPath(cur_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","Get path failed which Local AppData",QMessageBox::StandardButton::Abort);

        return(false);
    }

    cfg_filename=MergeFilename(cur_path,OS_TEXT("material_writer.json"));

    if(!FileExist(cfg_filename))
        return(false);

    {
        Json::Value root;
        OSString error_info;

        if(LoadJson(root,cfg_filename,error_info))
        {
            if(root.isMember("path"))
            {
                const Json::Value &path_root=root["path"];

            #define PARSE_JSON_STR(name)    if(path_root.isMember(#name))path_config.name=FixFilename(ToOSString(path_root[#name].asCString()));

                PARSE_JSON_STR(library)
                PARSE_JSON_STR(source)
                PARSE_JSON_STR(output)

            #undef PARSE_JSON_STR
            }
            
            //font
            if(root.isMember("font"))
            {
                const Json::Value &font_root=root["font"];
                
                if(font_root.isMember("ui"))
                    LoadFontJson(ui_fnt,font_root["ui"]);

                if(font_root.isMember("code"))
                    LoadFontJson(code_fnt,font_root["code"]);
            }
            else
            {
                QFont fnt;

                SetUIFont(fnt);
                SetCodeFont(fnt);
            }

            code_fnt.setFixedPitch(true);
            code_fnt.setStyleHint(QFont::StyleHint::Monospace);


            //ui
            if(root.isMember("ui"))
            {
                const Json::Value &ui_root=root["ui"];

                if(ui_root.isMember("style"))
                {
                    const std::string style=ui_root["style"].asString();

                    hgl::qt::SetApplicationStyle(QString::fromStdString(style));
                }
            }

            return(true);
        }

        QMessageBox::warning(nullptr,"Fatal Error","Load config file failed.",QMessageBox::StandardButton::Abort);
    }

    return(false);
}

Json::Value CreateFontJson(const QFont &fnt)
{
    Json::Value root;

    root["name"]=Json::Value(fnt.family().toStdString());
    root["size"]=Json::Value(fnt.pointSize());

    return root;
}

void SaveConfigData()
{
    UTF8StringList sl;

    Json::Value root;

    //path
    {
        Json::Value path_root;

        #define SET_JSON_STR(name)  if(!path_config.name.IsEmpty())path_root[#name]=Json::Value(ToUTF8String(path_config.name));

        SET_JSON_STR(library)
        SET_JSON_STR(source)
        SET_JSON_STR(output)

        #undef SET_JSON_STR

        root["path"]=path_root;
    }
    
    //font
    {
        Json::Value font_root;

        font_root["ui"]=CreateFontJson(ui_fnt);
        font_root["code"]=CreateFontJson(code_fnt);

        root["font"]=font_root;
    }

    //ui
    {
        Json::Value ui_root;

        ui_root["style"]=hgl::qt::GetApplicationStyle().toStdString();

        root["ui"]=ui_root;
    }
    
    OSString error_info;

    if(!SaveJson(root,cfg_filename,error_info))
    {
        QMessageBox::warning(nullptr,"Fatal Error",QString("can't save config file.\n\nfilename: ")+ToQString(cfg_filename),QMessageBox::StandardButton::Abort);
    }
}