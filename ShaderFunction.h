#pragma once

#include"ShaderSourceLocation.h"
#include<hgl/type/List.h>
#include<hgl/type/StringList.h>

using namespace hgl;

struct ShaderFunction
{
    UTF8String name;

    UTF8StringList identify_list;           ///<�õ��ı�ʶ���б�(�����Ǳ������ṹ��������const�����ȵ�)

    ShaderSourceLocation location;
};
