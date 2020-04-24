#include"ShaderDataType.h"

inline ShaderDataFormat MakeShaderDataFormat(const ShaderDataType type, const uint count)
{
    if (type<ShaderDataType::BEGIN_RANGE
     || type>ShaderDataType::END_RANGE)return 0;

    if (count < 1 || count>4)return 0;

    return((uint(type) << 2) | (count - 1));
}

UTF8String MakeValueName(const ShaderDataType type,const uint count)
{
    constexpr char type_name[][8]=
    {
        "bool",
        "int",
        "uint",
        "float",
        "double"
    };

    constexpr char type_vec_name[][8]=
    {
        "bvec",
        "ivec",
        "uvec",
        "vec",
        "dvec"
    };

    if(type<ShaderDataType::BEGIN_RANGE
     ||type>ShaderDataType::END_RANGE)
        return("[ERROR ShaderDataType]");

    if(count<1||count>4)
        return("[ERROR Type component count]");

    if(count==1)
        return type_name[uint(type)];

    return UTF8String(type_vec_name[uint(type)])+UTF8String::valueOf(count);
}

UTF8String MakeValueName(const ShaderDataFormat sdf)
{
    uint type,count;

    type=sdf>>2;
    count=sdf&3;

    return MakeValueName((ShaderDataType)type,count+1);
}

const ShaderDataFormat ParseShaderType(const UTF8String &str)
{
    const char first_char=str.GetBeginChar();
    const char end_char=str.GetEndChar();

    if(first_char=='f')return MakeShaderDataFormat(ShaderDataType::Float,1);

    uint count=0;

    if(end_char>='2'&&end_char<='4')        //vec
        count=end_char-'0';
    else 
        count=1;

    if(first_char=='b')return MakeShaderDataFormat(ShaderDataType::Bool,    count);else
    if(first_char=='i')return MakeShaderDataFormat(ShaderDataType::Int,     count);else
    if(first_char=='u')return MakeShaderDataFormat(ShaderDataType::Uint,    count);else
    if(first_char=='v')return MakeShaderDataFormat(ShaderDataType::Float,   count);else
    if(first_char=='d')return MakeShaderDataFormat(ShaderDataType::Double,  count);else
        return 0;   
}