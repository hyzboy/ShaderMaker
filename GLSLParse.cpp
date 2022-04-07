#include<hgl/type/StringList.h>
#include"GLSLTokenizer.h"

using namespace hgl;
using namespace shader_lib;

namespace
{
	struct ShaderFileStatus
	{
		OSString filename;
		uint64 last_write_time;

		UTF8StringList struct_list;
		UTF8StringList function_list;
	};

	void ParseOnlineComment(const char *comment_text,const int comment_length)
	{
	}

	void ParseMultilineComment(const char *comment_text,const int comment_length)
	{
	}
}//namespace

bool ParseShader(const OSString &filename)
{
	UTF8String shader_text;

	if(LoadStringFromTextFile(shader_text,filename)<=0)
		return(false);

	GLSLTokenizer parse(shader_text.c_str(),shader_text.Length());
	GLSLToken token;
	int length;
	const char *str;

	bool is_struct=false;			//�Ƿ��ǽṹ
	bool is_function=false;			//�Ƿ�ʼ������
	bool is_function_param=false;	//�Ƿ�ʼ��������������
	bool is_param=false;			//�Ƿ��ڽ���һ������

	int statement_block_stack=0;	//�����Ų㼶

	GLSLToken function_return=GLSLToken::None;		//������������
	UTF8String function_name;

	GLSLToken param_inout=GLSLToken::None;
	GLSLToken param_type=GLSLToken::None;
	UTF8String param_type_name;
	UTF8String param_value;

	while((str=parse.GetToken(&token,&length)))
	{
		if(token==GLSLToken::WhiteSpace
		 ||token==GLSLToken::Return
		 ||token==GLSLToken::Enter)continue;

		if(token==GLSLToken::OnelineComment)
			ParseOnlineComment(str,length);
		else
		if(token==GLSLToken::MultilineComment)
			ParseMultilineComment(str,length);
		else
		if(token==GLSLToken::StartStatementBlock)	// {
		{
			if(statement_block_stack==0)
			{
				if(function_return!=GLSLToken::None)
				{
				}
				else
				if(is_struct)
				{
					LOG_INFO(U8_TEXT("Struct Begin: ")+function_name);
				}
			}

			++statement_block_stack;
		}
		else
		if(token==GLSLToken::EndStatementBlock)	// }
		{
			--statement_block_stack;

			if(statement_block_stack==0)
			{
				if(is_struct)
				{
					LOG_INFO(U8_TEXT("Struct End: ")+function_name);

					is_struct=false;
				}
				else
				if(is_function)
				{
					LOG_INFO(U8_TEXT("Function End: ")+function_name);

					function_return=GLSLToken::None;
					function_name.Clear();
					is_function=false;
				}
				else
				{
					// What was end?
				}
			}
		}
		else
		if(token==GLSLToken::OpenParanthesis)	// (
		{
			if(is_function
			 &&!is_function_param)
			{
				is_function_param=true;
			}
		}
		else
		if(token==GLSLToken::CloseParanthesis)	// )
		{
			if(is_param)
				is_param=false;

			if(is_function_param)
				is_function_param=false;

			param_type_name.Clear();
			param_value.Clear();
		}
		else
		if(token==GLSLToken::ListSeparator)		// ,
		{
			if(is_param)
			{
				is_param=false;				
				param_type_name.Clear();
				param_value.Clear();
			}
		}
		else
		if(token==GLSLToken::EndStatement)		// ;
		{
			if(is_struct)
			{
				is_param=false;
				param_type_name.Clear();
				param_value.Clear();
			}
		}
		else
		if(token==GLSLToken::Struct)
		{
			is_struct=true;
		}
		else
		if(token==GLSLToken::Const)
		{
			if(statement_block_stack<=0)
			{
				const char *const_start=str;
				int len=0;

				do
				{
					len+=length;
					str=parse.GetToken(&token,&length);
				}while(str&&token!=GLSLToken::EndStatement);

				if(token==GLSLToken::EndStatement)
					++len;

				UTF8String const_value(const_start,len);

				LOG_INFO(U8_TEXT("const value: ")+const_value);
			}
		}
		else
		if(token==GLSLToken::In
		 &&token==GLSLToken::InOut)
		{
			if(is_param)
				param_inout=token;
		}
		else
		if(token>=GLSLToken::Void
		 &&token<=GLSLToken::mat4x4)	//������������ֵ
		{
			if(statement_block_stack==0)		//û�л�����
			{
				if(is_function_param)			//������������
				{
					param_type=token;
					param_type_name.SetString(str,length);
					is_param=true;
				}
				else
				{
					function_return=token;
				}
			}
		}
		else
		if(token==GLSLToken::Identifier)
		{
			if(statement_block_stack==0)
			{
				if(is_struct)
				{
					if(param_type_name.IsEmpty())
					{
						param_type_name.SetString(str,length);
						is_param=true;
					}
					else
					if(is_param)
					{
						if(param_value.IsEmpty())
						{
							param_value.SetString(str,length);

							LOG_INFO(U8_TEXT("    struct param: ")+param_type_name+U8_TEXT(" "+param_value));
						}
						else
						{
							//why ?

							LOG_ERROR(U8_TEXT("Parse error at struct param, struct:")+function_name);
						}
					}
				}
				else
				if(is_function_param)
				{
					if(param_type_name.IsEmpty())
					{
						param_type_name.SetString(str,length);
						is_param=true;
					}
					else
					if(is_param)
					{
						if(param_value.IsEmpty())
						{
							param_value.SetString(str,length);

							LOG_INFO(U8_TEXT("    function param: ")+param_type_name+U8_TEXT(" "+param_value));
						}
						else
						{
							//why ?

							LOG_ERROR(U8_TEXT("Parse error at function param, function:")+function_name);
						}
					}
				}
				else
				if(function_return!=GLSLToken::None)
				{
					is_function=true;
					function_name.SetString(str,length);
					
					LOG_INFO(U8_TEXT("Function Begin: ")+function_name);
				}
			}
		}
	}

	return(true);
}
