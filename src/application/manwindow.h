#ifndef MANWINDOW_H
#define MANWINDOW_H

#include <QDialog>

namespace Ui {
    class ManWindow;
}

class ManWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManWindow(QWidget *parent = 0);
    ~ManWindow();

private:
    Ui::ManWindow *ui;
};

#endif // MANWINDOW_H
