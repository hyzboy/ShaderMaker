#pragma once

#include<hgl/type/EnumClass.h>

#define ML_COLUMN_NAME     0
#define ML_COLUMN_TYPE     1

enum class MaterialFileType
{ 
    Material,
    Vert,
    Tesc,
    Tese,
    Geom,
    Frag,
    Comp,
    Mesh,
    Task,
    Rgen,
    Rint,
    Rahit,
    Rchit,
    Rmiss,
    Rcall,

    Unknow,

    ENUM_CLASS_RANGE(Material,Rcall)
};

constexpr const char *MaterialFileTypeName[(size_t)MaterialFileType::RANGE_SIZE]=
{    
    "Material",
    "Vert",
    "Tesc",
    "Tese",
    "Geom",
    "Frag",
    "Comp",
    "Mesh",
    "Task",
    "Rgen",
    "Rint",
    "Rahit",
    "Rchit",
    "Rmiss",
    "Rcall"
};