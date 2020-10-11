#include<QMainWindow>
#include<QTreeWidgetItem>
#include<QListWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<hgl/type/StringList.h>

using namespace hgl;

class MainWindow:public QMainWindow
{
    Q_OBJECT

private:

    void about();

    void NewMaterialFile();
    void OpenMaterialFile();

    void PathConfig();

private:

    void InitMenu();

private:    // shaderlib tree

    typedef void (MainWindow::*ShaderLibItemClickedFunc)(QListWidgetItem *);

    QTextEdit *sl_preview;
    QSyntaxHighlighter *highlighter;

    void VaryingClicked(QListWidgetItem *item);
    void RawClicked(QListWidgetItem *item);
    void StructClicked(QListWidgetItem *item);
    void ModuleClicked(QListWidgetItem *item);

    QListWidget *CreateListWidget(const UTF8StringList &str_list,ShaderLibItemClickedFunc);

    QWidget *CreateShaderLibraryWidget();
    QWidget *CreateMaterialLibraryWidget();

public:

    using QMainWindow::QMainWindow;
    ~MainWindow()=default;

    void Init();
};//class MainWindow:public QMainWindow
