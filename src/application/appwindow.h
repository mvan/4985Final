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

private slots:
    void on_stream_clicked();
};

#endif // APPWINDOW_H
