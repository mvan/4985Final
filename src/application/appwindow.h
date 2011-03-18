#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QTabWidget>
#include <QFileDialog>
#include <QStringList>
#include <QTreeWidgetItem>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
#include <QList>

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
    QStringList treename;

    Phonon::MediaObject mediaObject;
    Phonon::AudioOutput *audioOutput;
    QList<Phonon::MediaSource> mediaSources;

    void setupGui();
};

#endif // APPWINDOW_H
