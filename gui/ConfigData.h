#pragma once

#include<hgl/type/String.h>
#include<QFont>

using namespace hgl;

bool LoadConfig();

const OSString &GetShaderLibraryPath();
const OSString &GetMaterialSourcePath();
const OSString &GetMaterialOutputPath();
const OSString &GetVaryingLibraryPath();

const QFont &GetUIFont();
const QFont &GetCodeFont();

void SetShaderLibraryPath(const OSString &path);
void SetMaterialSourcePath(const OSString &path);
void SetMaterialOutputPath(const OSString &path);

void SetUIFont(const QFont &fnt);
void SetCodeFont(const QFont &fnt);
void SaveConfigData();

