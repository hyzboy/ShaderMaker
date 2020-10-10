#pragma once

#include<hgl/type/String.h>

using namespace hgl;

bool LoadConfig();

const OSString &GetShaderLibPath();
const OSString &GetMaterialLibPath();
const OSString &GetMaterialOutputPath();

void SetShaderLibPath(const OSString &path);
void SetMaterialLibPath(const OSString &path);
void SetMaterialOutputPath(const OSString &path);
