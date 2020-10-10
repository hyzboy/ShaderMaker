#include"DLG_PathConfig.h"
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QFileDialog>

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

void DLGPathConfig::CreateGroup(QWidget *parent,int row,const QString &name)
{
    QLabel *lab=new QLabel(parent);
    lab->setText(name);

    QLineEdit *edit=new QLineEdit(parent);
    edit->setMinimumWidth(512);
    edit->setReadOnly(true);

    BrowserButton *but=new BrowserButton(parent,edit);

    grid_layout->addWidget(lab, row,0,Qt::AlignRight);
    grid_layout->addWidget(edit,row,1);
    grid_layout->addWidget(but, row,2);

    path_edit[row]=edit;
}

DLGPathConfig::DLGPathConfig()
{
    setModal(true);
    setWindowTitle(tr("path config"));

    QWidget *widget=new QWidget(this);
    QVBoxLayout *layout=new QVBoxLayout(widget);

    {
        QWidget *grid_widget=new QWidget(widget);
        grid_layout=new QGridLayout(grid_widget);

        {
            CreateGroup(grid_widget,0,"ShaderLib path:");
            CreateGroup(grid_widget,1,"Material path:");
            CreateGroup(grid_widget,2,"Output path:");
        }

        layout->addWidget(grid_widget);
    }

    resize(layout->sizeHint());
}
