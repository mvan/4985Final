#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QTabWidget>
#include <QFileDialog>
#include <QStringList>
#include <QTreeWidgetItem>

namespace Ui {
    class AppWindow;
}

class AppWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();

public slots:
    void addFiles();
    void playPause();
    void onOffMicOther();
    void onOffMicSelf();

private:
    Ui::AppWindow *ui;
    QFileDialog *fd;
    QTreeWidgetItem *lstItem;
    QStringList filenames;
};

#endif // APPWINDOW_H
