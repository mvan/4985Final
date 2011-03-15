#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QTabWidget>

namespace Ui {
    class AppWindow;
}

class AppWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();

private:
    Ui::AppWindow *ui;
};

#endif // APPWINDOW_H
