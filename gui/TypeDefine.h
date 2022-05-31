#pragma once

#include<hgl/type/EnumClass.h>

#define ML_COLUMN_NAME     0
#define ML_COLUMN_TYPE     1

enum class MaterialFileType
{ 
    None=0,

    Material,
    Vert,
    Geom,
    Frag,

    ENUM_CLASS_RANGE(None,Frag)
};

constexpr const char *MaterialFileTypeName[16]=
{
    "None",
    "Material",
    "Vert",
    "Geom",
    "Frag",
};