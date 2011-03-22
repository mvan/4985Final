#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QDialog>
#include <winsock2.h>
#include <QMessageBox>

#include "clientwindow.h"

namespace Ui {
    class connectDlg;
}

class connectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit connectDlg(QWidget *parent = 0);
    ~connectDlg();

private slots:
    void connectToSrv();

private:
    Ui::connectDlg *ui;
    bool connectToTcp();
    bool connectToUdp();
};

#endif // CONNECTDLG_H
