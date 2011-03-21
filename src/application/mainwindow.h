#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class AboutWindow;
class ManWindow;
class AppWindow;
class ConnectionControl;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openAbout();
    void openManual();
    void openApp();

private:
    Ui::MainWindow *ui;
    AboutWindow *aw;
    ManWindow *mw;
    AppWindow *apw;
    ConnectionControl *control_;
};

#endif // MAINWINDOW_H
