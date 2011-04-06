/**
  * Class: AboutWindow
  *
  * Description:
  * AboutWindow defines the window with information about the program,
  * and the authors.
  */

#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QFile>

namespace Ui {
    class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

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
    /** The user interface of AboutWindow */
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
