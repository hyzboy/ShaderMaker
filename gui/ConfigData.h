#pragma once

#include<hgl/type/String.h>
#include<QFont>

using namespace hgl;

bool LoadConfig();

const OSString &GetShaderLibraryPath();
const OSString &GetMaterialSourcePath();
const OSString &GetMaterialOutputPath();

const QFont &GetUIFont();
const QFont &GetCodeFont();
