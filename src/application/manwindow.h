/**
  * Class: ManWindow
  *
  * Description:
  * ManWindow shows the user manual or basic how-to's for using the program.
  */

#ifndef MANWINDOW_H
#define MANWINDOW_H

#include <QDialog>
#include <QFile>

namespace Ui {
    class ManWindow;
}

class ManWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManWindow(QWidget *parent = 0);
    ~ManWindow();

    /**
      * Function: applyStylesheet(QString path)
      *
      * @param path filepath of the stylesheet
      *
      * Description:
      * This function applies the stylesheet specified by path
      * to the current window.
      */
    void applyStyleSheet(QString path);

private:
    /** The user interface of ManWindow */
    Ui::ManWindow *ui;
};

#endif // MANWINDOW_H
