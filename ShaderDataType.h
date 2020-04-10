#pragma once

#include<hgl/type/List.h>
#include<hgl/type/BaseString.h>

using namespace hgl;

constexpr size_t MAX_SHADER_VALUE_NAME_LENGTH = 32;

enum class ShaderDataType
{
    Bool = 0,
    Float,
    Int,
    Uint,
    Double,

    BEGIN_RANGE = Bool,
    END_RANGE = Double,
    RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
};//

using ShaderDataFormat = uint32;

inline ShaderDataFormat MakeShaderDataFormat(const ShaderDataType type, const uint count);
UTF8String MakeValueName(const ShaderDataType type,const uint count);
UTF8String MakeValueName(const ShaderDataFormat sdf);

const ShaderDataFormat ParseShaderType(const UTF8String &str);
    
struct ShaderAttribute
{
    ShaderDataFormat format;
    char name[MAX_SHADER_VALUE_NAME_LENGTH+1];
};

using ShaderAttributeList=List<ShaderAttribute>;
