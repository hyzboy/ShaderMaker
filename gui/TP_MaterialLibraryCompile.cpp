#include"TP_MaterialLibrary.h"
#include"ShaderLib.h"

void TPMaterialLibrary::OnConvertToGLSL()
{
    if(!current_item)return;

    QString type=current_item->text(ML_COLUMN_TYPE);

    if(type==ML_TYPE_VERTEX
     ||type==ML_TYPE_GEOMETRY
     ||type==ML_TYPE_FRAGMENT)
    {
        OnSave();

        shader_lib::LoadXMLShader(filename,
    }
}

void TPMaterialLibrary::OnConvertAndCompile()
{
}

void TPMaterialLibrary::OnBuild()
{
}