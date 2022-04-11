#include"GLSLTokenizer.h"
#include<string.h>

namespace shader_lib
{
	namespace
	{
        struct TokenWord
	    {
		    const char *word;
		    GLSLToken token;
	    };

	    constexpr TokenWord glsl_token_words[]=
	    {
            {"+"        , GLSLToken::Plus},
            {"-"        , GLSLToken::Minus},
            {"*"        , GLSLToken::Star},
            {"/"        , GLSLToken::Slash},
            {"%"        , GLSLToken::Percent},
            {"#"        , GLSLToken::Handle},
            {"&"        , GLSLToken::Amp},
            {"!"        , GLSLToken::Not},
        
            {"+="       , GLSLToken::AddAssign},
            {"-="       , GLSLToken::SubAssign},
            {"*="       , GLSLToken::MulAssign},
            {"/="       , GLSLToken::DivAssign},
            {"%="       , GLSLToken::ModAssign},

            {"|="       , GLSLToken::OrAssign},
            {"&="       , GLSLToken::AndAssign},
            {"^="       , GLSLToken::XorAssign},
            {"<<="      , GLSLToken::ShiftLeftAssign},
            {">>="      , GLSLToken::ShiftRightAssign},
        
            {"++"       , GLSLToken::Inc},
            {"--"       , GLSLToken::Dec},
            {"."        , GLSLToken::Dot},

            {"="        , GLSLToken::Assignment},
            {";"        , GLSLToken::EndStatement},
            {","        , GLSLToken::ListSeparator},
        
            {"{"        , GLSLToken::StartStatementBlock},
            {"}"        , GLSLToken::EndStatementBlock},
            {"("        , GLSLToken::OpenParanthesis},
            {")"        , GLSLToken::CloseParanthesis},
            {"["        , GLSLToken::OpenBracket},
            {"]"        , GLSLToken::CloseBracket},

            {"|"        , GLSLToken::BitOr},
            {"~"        , GLSLToken::BitNot},
            {"^"        , GLSLToken::BitXor},
            {"<<"       , GLSLToken::BitShiftLeft},
            {">>"       , GLSLToken::BitShiftRight},

            {"=="       , GLSLToken::Equal},
            {"!="       , GLSLToken::NotEqual},
            {"<"        , GLSLToken::LessThan},
            {">"        , GLSLToken::GreaterThan},
            {"<="       , GLSLToken::LessThanOrEqual},
            {">="       , GLSLToken::GreaterThanOrEqual},

            {"?"        , GLSLToken::Question},
            {":"        , GLSLToken::Colon},
            {"||"       , GLSLToken::Or},
            {"&&"       , GLSLToken::And},

            {"if"       , GLSLToken::If},
            {"else"     , GLSLToken::Else},
            {"for"      , GLSLToken::For},
            {"do"       , GLSLToken::Do},
            {"while"    , GLSLToken::While},
            {"goto"     , GLSLToken::Goto},
            {"break"    , GLSLToken::Break},
            {"continue" , GLSLToken::Continue},
            {"switch"   , GLSLToken::Switch},
            {"case"     , GLSLToken::Case},
            {"default"  , GLSLToken::Default},

            {"import"   , GLSLToken::Import},
            {"include"  , GLSLToken::Include},

            {"true"     , GLSLToken::True},
            {"false"    , GLSLToken::False},

            {"in"       , GLSLToken::In},
            {"out"      , GLSLToken::Out},
            {"inout"    , GLSLToken::InOut},

            {"struct"   , GLSLToken::Struct},
            {"uniform"  , GLSLToken::Uniform},

            {"void"     , GLSLToken::Void},
            {"bool"     , GLSLToken::Boolean},
            {"int"      , GLSLToken::Integer},
            {"uint"     , GLSLToken::UInteger},
            {"float"    , GLSLToken::Float},
            {"double"   , GLSLToken::Double},

            {"const"    , GLSLToken::Const},
            {"shared"   , GLSLToken::Shared},

            {"coherent" , GLSLToken::Coherent},
            {"volatile" , GLSLToken::Volatile},
            {"restrict" , GLSLToken::Restrict},
            {"readonly" , GLSLToken::ReadOnly},
            {"writeonly", GLSLToken::WriteOnly},

            {"precision", GLSLToken::Precision},
            {"invariant", GLSLToken::Invariant},
            {"patch"    , GLSLToken::Patch},

            {"flat"         ,GLSLToken::Flat},
            {"noperspective",GLSLToken::Noperspective},
            {"smooth"       ,GLSLToken::Smooth},

            {"subroutine",GLSLToken::Subroutine},

            #define GLSL_RAW_TYPE(name) {#name,GLSLToken::name}

                #define GLSL_VEC_TYPE(name) GLSL_RAW_TYPE(name),GLSL_RAW_TYPE(name##1),GLSL_RAW_TYPE(name##2),GLSL_RAW_TYPE(name##3),GLSL_RAW_TYPE(name##4)
                    GLSL_VEC_TYPE(vec),
                    GLSL_VEC_TYPE(bvec),
                    GLSL_VEC_TYPE(ivec),
                    GLSL_VEC_TYPE(uvec),
                    GLSL_VEC_TYPE(dvec),
                #undef GLSL_VEC_TYPE

                #define GLSL_MAT_TYPE(name) GLSL_RAW_TYPE(mat##name),GLSL_RAW_TYPE(mat2x##name),GLSL_RAW_TYPE(mat3x##name),GLSL_RAW_TYPE(mat4x##name)
                    GLSL_MAT_TYPE(2),
                    GLSL_MAT_TYPE(3),
                    GLSL_MAT_TYPE(4),
                #undef GLSL_MAT_TYPE

                GLSL_RAW_TYPE(row_major),
		        GLSL_RAW_TYPE(column_major),

		        GLSL_RAW_TYPE(origin_upper_left),
		        GLSL_RAW_TYPE(pixel_center_integer),

		        GLSL_RAW_TYPE(sampler),
		        GLSL_RAW_TYPE(sampler1D),
		        GLSL_RAW_TYPE(sampler2D),
		        GLSL_RAW_TYPE(sampler3D),
		        GLSL_RAW_TYPE(samplerCube),
		        GLSL_RAW_TYPE(sampler2DRect),
		        GLSL_RAW_TYPE(sampler1DArray),
		        GLSL_RAW_TYPE(sampler2DArray),
		        GLSL_RAW_TYPE(samplerCubeArray),
		        GLSL_RAW_TYPE(samplerBuffer),
		        GLSL_RAW_TYPE(sampler2DMS),
		        GLSL_RAW_TYPE(sampler2DMSArray),

		        GLSL_RAW_TYPE(sampler1DShadow),
		        GLSL_RAW_TYPE(sampler2DShadow),
		        GLSL_RAW_TYPE(samplerCubeShadow),
		        GLSL_RAW_TYPE(sampler2DRectShadow),
		        GLSL_RAW_TYPE(sampler1DArrayShadow),
		        GLSL_RAW_TYPE(sampler2DArrayShadow),
		        GLSL_RAW_TYPE(samplerCubeArrayShadow),

		        GLSL_RAW_TYPE(buffer),

		        GLSL_RAW_TYPE(depth_any),
		        GLSL_RAW_TYPE(depth_greater),
		        GLSL_RAW_TYPE(depth_less),
		        GLSL_RAW_TYPE(depth_unchanged)
            #undef GLSL_RAW_TYPE
	    };//constexpr TokenWord glsl_token_words[]=

        const int GLSLTokenWordsNumber=sizeof(glsl_token_words)/sizeof(TokenWord);
	}//namespace
    
    GLSLTokenizer::GLSLTokenizer(const char *shader_source, int length)
    {

        this->source = shader_source;
        this->source_length = length;
    }

    const char *GLSLTokenizer::GetToken(GLSLToken *type, int *length)
    {
        if(!source
         ||source_length<=0
         ||!type
         ||!length)return(nullptr);

        ParseToken();

        if(this->token_type==GLSLToken::None)
            return(nullptr);

        *length = this->token_length;
        *type   = this->token_type;
        
        const char *result=source;
        
        source+=token_length;
        source_length-=token_length;
        
        return result;
    }
	
    int GLSLTokenizer::ParseToken()
    {
        if( IsWhiteSpace() ) return 0;
        if( IsLine()       ) return 0;
        if( IsComment()    ) return 0;
        if( IsConstant()   ) return 0;
        if( IsIdentifier() ) return 0;
        if( IsKeyWord()    ) return 0;

        token_type = GLSLToken::None;
        token_length = 1;

        return -1;
    }
    
    bool GLSLTokenizer::IsWhiteSpace()
    {
        int n;

        for( n = 0; n < source_length; n++ )
        {
            if( source[n] != ' '
              &&source[n] != '\t')
                break;
        }

        if( n > 0 )
        {
            token_type = GLSLToken::WhiteSpace;
            token_length = n;
            return true;
        }

        return false;
    }

    bool GLSLTokenizer::IsLine()
    {
        int n=0;

        for(n=0;n<source_length;n++)
        {
            if(source[n]!='\r'
             &&source[n]!='\n')
                break;
        }

        if(n>0)
        {
            token_type=GLSLToken::NewLine;
            token_length=n;
            return(true);
        }

        return false;
    }
    
    bool GLSLTokenizer::IsComment()
    {
        if( source_length < 2 )
            return false;

        if( source[0] != '/' )
            return false;

        if( source[1] == '/' )
        {
            // One-line comment

            // Find the length
            int n;
            for( n = 2; n < source_length; n++ )
            {
                if( source[n] == '\n' )
                    break;
            }

            token_type = GLSLToken::OnelineComment;
            token_length = n+1;

            return true;
        }

        if( source[1] == '*' )
        {
            // Multi-line comment

            // Find the length
            int n;
            for( n = 2; n < source_length-1; )
            {
                if( source[n++] == '*' && source[n] == '/' )
                    break;
            }

            token_type = GLSLToken::MultilineComment;
            token_length = n+1;

            return true;
        }

        return false;
    }

    bool GLSLTokenizer::IsConstant()
    {
        // Starting with number
        if( source[0] >= '0' && source[0] <= '9' )
        {
            // Is it a hexadecimal number?
            if( source_length >= 1 && (source[1] == 'x' || source[1] == 'X') )
            {
                int n;
                for( n = 2; n < source_length; n++ )
                {
                    if( !(source[n] >= '0' && source[n] <= '9') &&
                        !(source[n] >= 'a' && source[n] <= 'f') &&
                        !(source[n] >= 'A' && source[n] <= 'F') )
                        break;
                }

                token_type = GLSLToken::BitsConstant;
                token_length = n;
                return true;
            }

            int n;
            for( n = 1; n < source_length; n++ )
            {
                if( source[n] < '0' || source[n] > '9' )
                    break;
            }

            if( n < source_length && source[n] == '.' )
            {
                n++;
                for( ; n < source_length; n++ )
                {
                    if( source[n] < '0' || source[n] > '9' )
                        break;
                }

                if( n < source_length && (source[n] == 'e' || source[n] == 'E') )
                {
                    n++;
                    if( n < source_length && (source[n] == '-' || source[n] == '+') )
                        n++;

                    for( ; n < source_length; n++ )
                    {
                        if( source[n] < '0' || source[n] > '9' )
                            break;
                    }
                }

                if( n < source_length && (source[n] == 'f' || source[n] == 'F') )
                {
                    token_type = GLSLToken::FloatConstant;
                    token_length = n + 1;
                }
                else
                {
                    token_type = GLSLToken::FloatConstant;
                    token_length = n;
                }
                return true;
            }

            token_type = GLSLToken::IntConstant;
            token_length = n;
            return true;
        }

        // Character literal between single-quotes
        if( source[0] == '\'' )
        {
            bool evenSlashes = true;
            int n;
            for( n = 1; n < source_length; n++ )
            {
                if( source[n] == '\n' ) break;
                if( source[n] == '\'' && evenSlashes )
                {
                    token_type = GLSLToken::IntConstant;
                    token_length = n+1;
                    return true;
                }
                if( source[n] == '\\' ) evenSlashes = !evenSlashes; else evenSlashes = true;
            }

            return false;
        }

        // UTF16String constant between double-quotes
        if( source[0] == '"' )
        {
            // Is it a normal string constant or a heredoc string constant?
            if( source_length >= 6 && source[1] == '"' && source[2] == '"' )
            {
                // Heredoc string constant (spans multiple lines, no escape sequences)

                // Find the length
                int n;
                for( n = 3; n < source_length-2; n++ )
                {
                    if( source[n] == '"' && source[n+1] == '"' && source[n+2] == '"' )
                        break;
                }

                return(false);
            }
            else
            {
                // Normal string constant
                bool evenSlashes = true;
                int n;
                for( n = 1; n < source_length; n++ )
                {
                    if( source[n] == '\n' ) break;
                    if( source[n] == '"' && evenSlashes )
                    {
                        token_type = GLSLToken::StringConstant;
                        token_length = n+1;
                        return true;
                    }
                    if( source[n] == '\\' ) evenSlashes = !evenSlashes; else evenSlashes = true;
                }

                return(false);
            }

            return true;
        }

        return false;
    }

    bool GLSLTokenizer::IsIdentifier()
    {
        // Starting with letter or underscore
        if( (source[0] >= 'a' && source[0] <= 'z') ||
            (source[0] >= 'A' && source[0] <= 'Z') ||
            (source[0] == '_') )
        {
            token_type = GLSLToken::Identifier;
            token_length = 1;

            for( int n = 1; n < source_length; n++ )
            {
                if( (source[n] >= 'a' && source[n] <= 'z') ||
                    (source[n] >= 'A' && source[n] <= 'Z') ||
                    (source[n] >= '0' && source[n] <= '9') ||
                    (source[n] == '_') )
                    token_length++;
                else
                    break;
            }

            // Make sure the identifier isn't a reserved keyword
            if( token_length > 50 ) return true;

            char test[51];
            memcpy(test, source, token_length);
            test[token_length] = 0;

            for( int i = 0; i < GLSLTokenWordsNumber; i++ )
            {
                //if( strcmp(test, glsl_token_words[i].word) == 0 )
                if( strcmp(test, glsl_token_words[i].word) == 0 )
                    return false;
            }

            return true;
        }

        return false;
    }

    bool GLSLTokenizer::IsKeyWord()
    {
        // Fill the list with all possible keywords
        // Check each character against all the keywords in the list,
        // remove keywords that don't match. When only one remains and
        // it matches the source completely we have found a match.
        int words[GLSLTokenWordsNumber];
        int n;
        for( n = 0; n < GLSLTokenWordsNumber; n++ )
            words[n] = n;

        int numWords = GLSLTokenWordsNumber;
        int lastPossible = -1;

        n = 0;
        while( n < source_length && numWords >= 0 )
        {
            for( int i = 0; i < numWords; i++ )
            {
                if( glsl_token_words[words[i]].word[n] == '\0' )
                {
                    if( numWords > 1 )
                    {
                        lastPossible = words[i];
                        words[i--] = words[--numWords];
                        continue;
                    }
                    else
                    {
                        token_type = glsl_token_words[words[i]].token;
                        token_length = n;
                        return true;
                    }
                }

                if( glsl_token_words[words[i]].word[n] != source[n] )
                {
                    words[i--] = words[--numWords];
                }
            }
            n++;
        }

        // The source length ended or there where no more matchable words
        if( numWords )
        {
            // If any of the glsl_token_words also end at this
            // position then we have found the matching token
            for( int i = 0; i < numWords; i++ )
            {
                if( glsl_token_words[words[i]].word[n] == '\0' )
                {
                    token_type = glsl_token_words[words[i]].token;
                    token_length = n;
                    return true;
                }
            }
        }

        // It is still possible that a shorter token was found
        if( lastPossible > -1 )
        {
            token_type = glsl_token_words[lastPossible].token;
            //token_length = strlen(glsl_token_words[lastPossible].word);
            token_length = strlen(glsl_token_words[lastPossible].word);
            return true;
        }

        return false;
    }
}//namespace shader_lib
