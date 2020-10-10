#include"ConfigData.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/
#include<QMessageBox>

using namespace hgl;

namespace
{
    OSString shader_lib_path;
    OSString material_lib_path;
    OSString material_output_path;
}//namespace

const OSString &GetShaderLibPath(){return shader_lib_path;}
const OSString &GetMaterialLibPath(){return material_lib_path;}
const OSString &GetMaterialOutputPath(){return material_output_path;}

void SetShaderLibPath(const OSString &path){shader_lib_path=path;}
void SetMaterialLibPath(const OSString &path){material_lib_path=path;}
void SetMaterialOutputPath(const OSString &path){material_output_path=path;}

bool LoadConfig()
{
    OSString cur_path;

    if(!filesystem::GetLocalAppdataPath(cur_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","Get path failed which Local AppData",QMessageBox::StandardButton::Abort);

        return(false);
    }

    const OSString filename=filesystem::MergeFilename(cur_path,OS_TEXT("shader_libs.config"));

    if(!filesystem::FileExist(filename))
        return(false);

    

    return(false);
}
