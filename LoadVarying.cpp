#include<hgl/filesystem/Filesystem.h>
#include<hgl/filesystem/EnumFile.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"
#include"GLSLTokenizer.h"

namespace shader_lib
{
    namespace
    {
        UTF8StringList varying_list;
        MapObject<UTF8String,VaryingConfig> varying_config_list;
    }//namespace

    const UTF8StringList &GetVaryingList()
    {
        return varying_list;
    }

    bool CheckVarying(const UTF8String &name)
    {
        return varying_config_list.KeyExist(name);
    }
        
    bool CheckVarying(const UTF8StringList &vary_list,InfoOutput *info_output)
    {
        const int count=vary_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
        {
            const auto &vary=vary_list.GetString(i);

            if(!CheckVarying(vary))
            {
                if(info_output)
                    info_output->colorWriteln("red","can't find Varing: "+vary);

                return(false);
            }
        }

        return(true);
    }

    VaryingConfig *GetVarying(const UTF8String &name)
    {
        return varying_config_list[name];
    }

    namespace
    {
        Varying *ParseVarying(const UTF8String &str)
        {
            GLSLTokenizer parse(str.c_str(),str.Length());
            
            Varying *v=new Varying;

            GLSLToken token;
            int length;

            const char *p=parse.GetToken(&token,&length);

            if(token==GLSLToken::Flat
             ||token==GLSLToken::Noperspective
             ||token==GLSLToken::Smooth)
                v->interpolation.SetString(p,length);

            do
            {
                if(token>GLSLToken::Void
                &&token<=GLSLToken::dvec4)
                {
                    v->type.SetString(p,length);
                    break;
                }

                p=parse.GetToken(&token,&length);
            }while(p);

            do
            {
                p=parse.GetToken(&token,&length);
                if(token==GLSLToken::Identifier)
                {
                    v->name.SetString(p,length);
                    break;
                }
            }while(p);

            do
            {
                p=parse.GetToken(&token,&length);
                if(token==GLSLToken::OnelineComment)
                {
                    v->comment.SetString(p+2,length-2);
                    return v;
                }
            }while(p);

            if(!v->name.IsEmpty()
             &&!v->type.IsEmpty())
            {
            #ifdef _DEBUG
                UTF8String info="  Varying: ";
                
                if(!v->interpolation.IsEmpty())
                {
                    info+=v->interpolation;
                    info+=" ";
                }

                info+=v->type;
                info+=" ";
                info+=v->name;

                if(!v->comment.IsEmpty())
                {
                    info+=";    //";
                    info+=v->comment;
                }

                LOG_INFO(info.c_str());
            #endif//_DEBUG

                return v;
            }

            delete v;
            return(nullptr);
        }
    }//namespace

    bool LoadVaryingFile(const hgl::OSString &filename)
    {
        UTF8StringList sl;

        int line_count=LoadStringListFromTextFile(sl,filename);

        LOG_INFO(OS_TEXT("Loading varying file: ")+filename);

        if(line_count<=0)return(false);

        VaryingConfig *vc=new VaryingConfig;

        for(int i=0;i<line_count;i++)
        {
            Varying *var;

            var=ParseVarying(sl[i]);
            if(var)vc->Add(var);
        }

        if(vc->GetCount()<=0)
        {
            delete vc;
            return(false);
        }

        {
            const UTF8String mfn=ToUTF8String(filesystem::ClipFileMainname(filename));

            varying_list.Add(mfn);

            varying_config_list.Add(mfn,vc);

            return(true);
        }
    }
    
    namespace
    {
        using namespace hgl;
        using namespace hgl::filesystem;

        class EnumVaryingFile:public EnumFile
        {
        public:

            void ProcFile(struct EnumFileConfig *,FileInfo &fi) override
            {
                const OSString &ext_name=ClipFileExtName(OSString(fi.name),false);

                if(ext_name.CaseComp(OS_TEXT("varying"))==0)
                    LoadVaryingFile(fi.fullname);
            }
        };//class EnumVaryingFile:public filesystem::EnumFile
    }//namespace

    bool LoadAllVarying(const OSString &path)
    {
        varying_list.Clear();
        varying_config_list.Clear();

        if(!filesystem::IsDirectory(path))
            return(false);

        filesystem::EnumFileConfig efc(path);
        efc.proc_folder=false;
        efc.find_name=OS_TEXT("*.varying");

        EnumVaryingFile evf;

        evf.Enum(&efc);

        return(true);
    }
}//namespace shader_lib
