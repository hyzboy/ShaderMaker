#pragma once
#include<hgl/type/StringList.h>

using namespace hgl;

namespace shader_lib
{
    int Load(const OSString &);
    void Clear();

    bool AddTo(UTF8StringList &,const int index);
}//namespace shader_lib