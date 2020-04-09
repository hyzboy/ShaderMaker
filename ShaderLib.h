#pragma once
#include<hgl/type/StringList.h>

using namespace hgl;

int LoadShader(const OSString &);
void ClearShaderLib();

bool AddShader(UTF8StringList &,const int index);
