#pragma once

#include<hgl/type/String.h>

using namespace hgl;

bool LoadConfig();

const OSString &GetShaderLibraryPath();
const OSString &GetMaterialSourcePath();
const OSString &GetMaterialOutputPath();
