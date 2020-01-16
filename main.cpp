#include"glsl2spv.h"
#include<hgl/util/cmd/CmdParse.h>

using namespace hgl;

enum class TokenType
{
    Unrecognized=0,

    End,

    WhiteSpace,         // ' ','\t','\r','\n'
    OnelineComment,     // //
    MultilineComment,   // /* */

    // Atoms
    Identifier,         // abc123
    Integer,            // 1234
    Float,              // 12.34
    String,             // "123"
    RawString,          // """text"""

    // Math operators
    Plus,               // +
    Minus,              // -
    Star,               // *
    Slash,              // /
    Percent,            // %

    Handle,             // #
    At,                 // @

    AddAssign,          // +=
    SubAssign,          // -=
    MulAssign,          // *=
    DivAssign,          // /=
    ModAssign,          // %=

    OrAssign,           // |=
    AndAssign,          // &=
    XorAssign,          // ^=
    ShiftLeftAssign,    // <<=
    ShiftRightAssign,   // >>=

    Inc,                // ++
    Dec,                // --

    Dot,                // .

    // Statement tokens
    Assignment,         // =
    EndStatement,       // ;
    ListSeparator,      // ,
    StartStatementBlock,// {
    EndStatementBlock,  // }
    OpenParanthesis,    // (
    CloseParanthesis,   // )
    OpenBracket,        // [
    CloseBracket,       // ]
    Amp,                // &

    Not,                // !
    Or,                 // ||
    And,                // &&
    Xor,                // ^^    

    // Bitwise operators
    BitOr,              // |
    BitNot,             // ~
    BitXor,             // ^
    BitShiftLeft,       // <<
    BitShiftRight,      // >>

    // Compare operators
    Equal,              // ==
    NotEqual,           // !=
    LessThan,           // <
    GreaterThan,        // >
    LessThanOrEqual,    // <=
    GreaterThanOrEqual, // >=

    Question,           // ?
    Colon,              // :
};//

struct TokenWord
{
    const char *word;
    uint        work_size;
    uint        token_type;
};

constexpr TokenWord token_words[]=
{
#define DEFINE_TOKEN(str,type)  {str,sizeof(str)-1,uint(TokenType::type)}

    DEFINE_TOKEN("+",   Plus),
    DEFINE_TOKEN("-",   Minus),
    DEFINE_TOKEN("*",   Star),
    DEFINE_TOKEN("/",   Slash),
    DEFINE_TOKEN("%",   Percent),
    DEFINE_TOKEN("=",   Assignment),
    DEFINE_TOKEN(".",   Dot),
    DEFINE_TOKEN("+=",  AddAssign),
    DEFINE_TOKEN("-=",  SubAssign),
    DEFINE_TOKEN("*=",  MulAssign),
    DEFINE_TOKEN("/=",  DivAssign),
    DEFINE_TOKEN("%=",  ModAssign),
    DEFINE_TOKEN("|=",  OrAssign),
    DEFINE_TOKEN("&=",  AndAssign),
    DEFINE_TOKEN("^=",  XorAssign),
    DEFINE_TOKEN("<<=", ShiftLeftAssign),
    DEFINE_TOKEN(">>=", ShiftRightAssign),
    DEFINE_TOKEN("|",   BitOr),
    DEFINE_TOKEN("~",   BitNot),
    DEFINE_TOKEN("^",   BitXor),
    DEFINE_TOKEN("<<",  BitShiftLeft),
    DEFINE_TOKEN(">>",  BitShiftRight),
    DEFINE_TOKEN(";",   EndStatement),
    DEFINE_TOKEN(",",   ListSeparator),
    DEFINE_TOKEN("{",   StartStatementBlock),
    DEFINE_TOKEN(")",   EndStatementBlock),
    DEFINE_TOKEN("(",   OpenParanthesis),
    DEFINE_TOKEN(")",   CloseParanthesis),
    DEFINE_TOKEN("[",   OpenBracket),
    DEFINE_TOKEN("]",   CloseBracket),
    DEFINE_TOKEN("?",   Question),
    DEFINE_TOKEN(":",   Colon),
    DEFINE_TOKEN("==",  Equal),
    DEFINE_TOKEN("!=",  NotEqual),
    DEFINE_TOKEN("<",   LessThan),
    DEFINE_TOKEN(">",   GreaterThan),
    DEFINE_TOKEN("<=",  LessThanOrEqual),
    DEFINE_TOKEN(">=",  GreaterThanOrEqual),
    DEFINE_TOKEN("++",  Inc),
    DEFINE_TOKEN("--",  Dec),
    DEFINE_TOKEN("&",   Amp),
    DEFINE_TOKEN("!",   Not),
    DEFINE_TOKEN("||",  Or),
    DEFINE_TOKEN("&&",  And),
    DEFINE_TOKEN("^^",  Xor),
    DEFINE_TOKEN("#",   Handle),
    DEFINE_TOKEN("@",   At)

#undef DEFINE_TOKEN
};

constexpr uint TokenWordsNumber    =sizeof(token_words)/sizeof(TokenWord);
constexpr char WhiteSpaces[]       =" \t\r\n";
constexpr uint WhiteSpacesNumber   =sizeof(WhiteSpaces)-1;

/**
 * 通用词法解析器
 */
class Tokenizer
{

public:

    
};//class WordsParse

enum class ShaderDataType
{
    Bool=0,
    Float,
    Int,
    Uint,
    Double,

    BEGIN_RANGE =Bool,
    END_RANGE   =Double,
    RANGE_SIZE  =(END_RANGE-BEGIN_RANGE+1)
};//

using ShaderDataFormat=uint32;

UTF8String MakeValueName(const ShaderDataType type,const uint count)
{
    constexpr char type_name[][8]=
    {
        "bool",
        "float",
        "int",
        "uint",
        "double"
    };

    constexpr char type_vec_name[][8]=
    {
        "bvec",
        "vec",
        "ivec",
        "uvec",
        "dvec"
    };

    if(type<ShaderDataType::BEGIN_RANGE
     ||type>ShaderDataType::END_RANGE)
        return("[ERROR ShaderDataType]");

    if(count<1||count>4)
        return("[ERROR Type component count]");

    if(count==1)
        return type_name[uint(type)];
}



#if HGL_OS == HGL_OS_Windows
int wmain(int argc,wchar_t **argv)
#else
int main(int argc,char **argv)
#endif//
{
    util::CmdParse cp(argc,argv);


}
