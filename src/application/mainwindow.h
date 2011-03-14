#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class AboutWindow;
class ManWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openAbout();
    void openManual();

private:
    Ui::MainWindow *ui;
    AboutWindow *aw;
    ManWindow *mw;
};

#endif // MAINWINDOW_H
