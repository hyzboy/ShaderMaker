#include"DLG_Config.h"
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QFileDialog>
#include<QFontDialog>
#include<QMessageBox>
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>
#include"ConfigData.h"

using namespace hgl;
using namespace hgl::filesystem;

class BrowserButton:public QPushButton
{
private:

    QLineEdit *edit;

private:

    void OnBrowser()
    {
        QString path_name=QFileDialog::getExistingDirectory(nullptr,"Select directory",edit->text(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

        if(path_name.size()<=0)
            return;

        edit->setText(path_name);
    }

public:

    BrowserButton(QWidget *parent,QLineEdit *le):QPushButton(parent)
    {
        edit=le;

        setText("Browser");

        connect(this,&QPushButton::clicked,this,&BrowserButton::OnBrowser);
    }
};//class BrowserButton:public QPushButton

void DLGConfig::CreateGroup(QWidget *parent,int row,const QString &name,const OSString &str)
{
    QLabel *lab=new QLabel(parent);
    lab->setText(name);

    QLineEdit *edit=new QLineEdit(parent);
    edit->setMinimumWidth(512);
    edit->setReadOnly(true);
    edit->setText(ToQString(str));

    BrowserButton *but=new BrowserButton(parent,edit);

    grid_layout->addWidget(lab, row,0,Qt::AlignRight);
    grid_layout->addWidget(edit,row,1);
    grid_layout->addWidget(but, row,2);

    path_edit[row]=edit;
}

void DLGConfig::UpdateFontButtonText(QPushButton *button,const QString &name,const QFont &fnt)
{
    QString text=QString("%1 Font: %2, %3 points").arg(name).arg(fnt.family()).arg(fnt.pointSize());
    button->setText(text);
}

QPushButton *DLGConfig::CreateFontConfig(QWidget *font_widget,QLayout *font_layout,const QString &name,const QFont &fnt,void (DLGConfig::*event_func)())
{
    QPushButton *button=new QPushButton(font_widget);
    
    connect(button,&QPushButton::clicked,this,event_func);

    UpdateFontButtonText(button,name,fnt);

    font_layout->addWidget(button);

    return button;
}

DLGConfig::DLGConfig()
{
    setModal(true);
    setWindowTitle(tr("Config"));

    QWidget *widget=new QWidget(this);
    QVBoxLayout *layout=new QVBoxLayout(widget);

    //三个目录“标签、输入框、浏览按钮”
    {
        QWidget *grid_widget=new QWidget(widget);
        grid_layout=new QGridLayout(grid_widget);

        {
            CreateGroup(grid_widget,0,"Shader Library path:",   GetShaderLibraryPath());
            CreateGroup(grid_widget,1,"Material Source path:",  GetMaterialSourcePath());
            CreateGroup(grid_widget,2,"Material Output path:",  GetMaterialOutputPath());
        }

        layout->addWidget(grid_widget);
    }

    //字体区
    {
        QWidget *font_widget=new QWidget(widget);
        QHBoxLayout *font_layout=new QHBoxLayout(font_widget);

        ui_font_button  =CreateFontConfig(font_widget,font_layout,"UI",     GetUIFont(),    &DLGConfig::OnUIFontClicked);
        code_font_button=CreateFontConfig(font_widget,font_layout,"Code",   GetCodeFont(),  &DLGConfig::OnCodeFontClicked);

        layout->addWidget(font_widget);
    }

    //按钮区
    {
        QWidget *button_widget=new QWidget(widget);
        QHBoxLayout *button_layout=new QHBoxLayout(button_widget);

        //确定、取消按钮
        {
            QPushButton *ok_button=new QPushButton(button_widget);
            ok_button->setText(tr("OK"));
            connect(ok_button,&QPushButton::clicked,this,&DLGConfig::OnOKClicked);

            button_layout->addWidget(ok_button,0,Qt::AlignRight);

            QPushButton *cancel_button=new QPushButton(button_widget);
            cancel_button->setText(tr("Cancel"));
            connect(cancel_button,&QPushButton::clicked,this,&DLGConfig::OnCancelClicked);
            
            button_layout->addWidget(cancel_button,0,Qt::AlignLeft);
        }

        layout->addWidget(button_widget);
    }

    resize(layout->sizeHint());
}

void DLGConfig::OnOKClicked()
{
    OSString s_path=ToOSString(path_edit[0]->text());
    OSString m_path=ToOSString(path_edit[1]->text());
    OSString o_path=ToOSString(path_edit[2]->text());

    if(!IsDirectory(s_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","can't find Shader Library path.",QMessageBox::StandardButton::Abort);

        return;
    }

    if(!IsDirectory(m_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","can't find Material Source path.",QMessageBox::StandardButton::Abort);

        return;
    }

    if(!IsDirectory(o_path))
    {
        QMessageBox::warning(nullptr,"Fatal Error","can't find Material Output path.",QMessageBox::StandardButton::Abort);

        return;
    }

    SetShaderLibraryPath(s_path);
    SetMaterialSourcePath(m_path);
    SetMaterialOutputPath(o_path);
    SaveConfigData();

    close();
}

void DLGConfig::OnCancelClicked()
{
    close();
}

void DLGConfig::OnUIFontClicked()
{
    bool ok;

    QFont font=GetUIFont();

    font=QFontDialog::getFont(&ok,font,this,"UI Font");

    if(ok)
    {
        SetUIFont(font);
        SaveConfigData();
        UpdateFontButtonText(ui_font_button,"UI",font);
    }
}

void DLGConfig::OnCodeFontClicked()
{
    bool ok;

    QFont font=GetCodeFont();

    font=QFontDialog::getFont(&ok,font,this,"Code Font");

    if(ok)
    {
        SetCodeFont(font);
        SaveConfigData();        
        UpdateFontButtonText(code_font_button,"Code",font);
    }
}