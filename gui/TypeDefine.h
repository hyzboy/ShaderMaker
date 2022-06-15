#pragma once

#include<hgl/type/EnumClass.h>

#define ML_COLUMN_NAME     0
#define ML_COLUMN_TYPE     1

enum class MaterialFileType
{ 
    None=0,

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

    END,

    ENUM_CLASS_RANGE(None,END)
};

constexpr const char *MaterialFileTypeName[(size_t)MaterialFileType::RANGE_SIZE]=
{
    "None",
    
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
    "Rcall",
    
    "END"
};