#include<QMainWindow>

class MainWindow:public QMainWindow
{
    Q_OBJECT

private:

    void about();

    void InitMenu();

public:

    using QMainWindow::QMainWindow;
    ~MainWindow()=default;

    void Init();
};//class MainWindow:public QMainWindow
