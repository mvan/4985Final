#ifndef STARTSERVERDLG_H
#define STARTSERVERDLG_H

#include <QDialog>

#include "serverwindow.h"

namespace Ui {
    class startServerDlg;
}

class startServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit startServerDlg(QWidget *parent = 0);
    ~startServerDlg();

private slots:
    bool startServer();

private:
    Ui::startServerDlg *ui;
};

#endif // STARTSERVERDLG_H
