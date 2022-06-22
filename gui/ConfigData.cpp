#include"ConfigData.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/type/StdString.h>
#include<hgl/io/TextOutputStream.h>
#include<QMessageBox>
#include<hgl/util/json/JsonTool.h>
#include<QApplication>
#include<QStyle>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

namespace
{
    OSString cfg_filename;

    OSString shader_library_path;
    OSString material_source_path;
    OSString material_output_path;

    QFont ui_fnt,code_fnt;
}//namespace

const OSString &GetShaderLibraryPath(){return shader_library_path;}
const OSString &GetMaterialSourcePath(){return material_source_path;}
const OSString &GetMaterialOutputPath(){return material_output_path;}

const QFont &GetUIFont()
{
    return ui_fnt;
}

const QFont &GetCodeFont()
{
    return code_fnt;
}

void SetShaderLibraryPath(const OSString &path){shader_library_path=FixFilename(path);}
void SetMaterialSourcePath(const OSString &path){material_source_path=FixFilename(path);}
void SetMaterialOutputPath(const OSString &path){material_output_path=FixFilename(path);}

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

    fnt.setFamily(QString::fromStdString(font_root["name"].asString()));
    fnt.setPointSize(font_root["size"].asUInt());
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
            #define PARSE_JSON_STR(name)    if(root.isMember(#name))name=FixFilename(ToOSString(root[#name].asCString()));

            PARSE_JSON_STR(shader_library_path  )
            PARSE_JSON_STR(material_source_path )
            PARSE_JSON_STR(material_output_path )
            
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

            #undef PARSE_JSON_STR

            //ui
            if(root.isMember("ui"))
            {
                const Json::Value &ui_root=root["ui"];

                if(ui_root.isMember("style"))
                {
                    const std::string style=ui_root["style"].asString();

                    QApplication::setStyle(QString::fromStdString(style));
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

    #define SET_JSON_STR(name)  root[#name]=Json::Value(ToUTF8String(name));

    SET_JSON_STR(shader_library_path)
    SET_JSON_STR(material_source_path)
    SET_JSON_STR(material_output_path)
    
    //font
    {
        Json::Value font_root;

        font_root["ui"]=CreateFontJson(ui_fnt);
        font_root["code"]=CreateFontJson(code_fnt);

        root["font"]=font_root;
    }

    #undef SET_JSON_STR

    //ui
    {
        Json::Value ui_root;

        ui_root["style"]=QApplication::style()->objectName().toStdString();

        root["ui"]=ui_root;
    }
    
    OSString error_info;

    if(!SaveJson(root,cfg_filename,error_info))
    {
        QMessageBox::warning(nullptr,"Fatal Error",QString("can't save config file.\n\nfilename: ")+ToQString(cfg_filename),QMessageBox::StandardButton::Abort);
    }
}