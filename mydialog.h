#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyDialog(QWidget *parent = 0);
    ~MyDialog();
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int day_distance_1(int year1,int month1,int day1,int year2,int month2,int day2);
public slots:
    void on_startButton_clicked();
private slots:
    void on_aboutButton_clicked();

    void on_helpButton_clicked();

private:
    Ui::MyDialog *ui;
    unsigned int source;
    unsigned int array[4][4];
    //方向键按下标志
    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;
    bool okPressKey;
    //MyDialog *dlg;
};

#endif // MYDIALOG_H
