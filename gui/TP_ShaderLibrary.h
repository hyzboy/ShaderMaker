#pragma once

#include<QTabWidget>
#include"TP_Varying.h"
#include"TP_Raw.h"
#include"TP_Struct.h"
#include"TP_Module.h"

class TPShaderLibrary:public QTabWidget
{
    Q_OBJECT

private:

    TPVarying *varying;
    TPRawGLSL *rawglsl;
    TPStruct *shader_struct;
    TPModule *shader_module;

public:

    TPShaderLibrary();
    ~TPShaderLibrary();
};//