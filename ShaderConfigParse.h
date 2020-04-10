#pragma once
#include<hgl/type/StringList.h>
#include"ShaderDataType.h"

using namespace hgl;

struct ShaderConfigParse
{
    ShaderAttributeList attr_list;
    ShaderAttributeList gbuffer_list;

    UTF8StringList attribute_to_gbuffer;
    UTF8StringList gbuffer_to_attribute;
};//

ShaderConfigParse *LoadShaderConfig(const OSString &);

