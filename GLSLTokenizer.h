#pragma once

#include"GLSLToken.h"

namespace shader_lib
{
	class GLSLTokenizer
	{
        const char *source;
        int source_length;

        GLSLToken token_type;
        int token_length;

	private:

		int ParseToken();

        bool IsWhiteSpace();
        bool IsLine();
        bool IsComment();
        bool IsConstant();
        bool IsKeyWord();
        bool IsIdentifier();

	public:

        GLSLTokenizer(const char *,int);

        const int GetLeftLength()const{return source_length;}                   ///<取得剩余代码总长度

		const char *GetToken(GLSLToken *,int *);
	};//class GLSLTokenizer
}//namespace shader_lib
