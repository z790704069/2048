#include <QTime>
#include <QKeyEvent>
#include <QDebug>
#include "mydialog.h"
#include "aboutdialog.h"
#include "helpdialog.h"
#include "ui_mydialog.h"
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <QMap>
#include <QDate>

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog),source(0),keyUp(false),keyDown(false),keyLeft(false),keyRight(false),okPressKey(false)
{
    ui->setupUi(this);
    ui->lossLabel->setText(" ");

    //QMap<QDate, QTextCharFormat> map = ui->calendarWidget->dateTextFormat();
    SYSTEMTIME sys;
    GetLocalTime( &sys );
    int year = sys.wYear;
    int month = sys.wMonth;
    int day = sys.wDay;
    int days = day_distance_1(year,month,day,2013,3,5);
    char date[10] = {0};
    itoa(days, date, 10);
    ui->DaysLove->setText(date);

    for(int i=0;i!=4;++i)
    {
        for(int j=0;j!=4;++j)
            array[i][j] = 0;//设置数组为无数状态 全为0
    }
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

int MyDialog::day_distance_1(int year1,int month1,int day1,int year2,int month2,int day2)
{
    struct tm tm1;
    tm1.tm_year = year1 - 1900;
    tm1.tm_mon = month1 - 1;
    tm1.tm_mday = day1;
    tm1.tm_hour = 0;
    tm1.tm_min = 0;
    tm1.tm_sec = 0;

    struct tm tm2;
    tm2.tm_year = year2 - 1900;
    tm2.tm_mon = month2 - 1;
    tm2.tm_mday = day2;
    tm2.tm_hour = 0;
    tm2.tm_min = 0;
    tm2.tm_sec = 0;

    time_t time1;
    time_t time2;
    time1 = mktime(&tm1);
    time2 = mktime(&tm2);
    double diff = difftime(time1,time2);
    return (int)(diff/(3600*24));
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::keyPressEvent(QKeyEvent *event)
{
    if(!okPressKey)
        return ;
       if(event->key()==Qt::Key_Up)
       {
           if(event->isAutoRepeat())
               return ;//按键重复时不做处理
           else
               keyUp = true;
           //qDebug()<<"Key_Up pressed";
       }
       else if(event->key()==Qt::Key_Down)
       {
           if(event->isAutoRepeat())
               return ;
           else
               keyUp = true;
           //qDebug()<<"Key_Down pressed";
       }
       else if(event->key()==Qt::Key_Left)
       {
           if(event->isAutoRepeat())
               return ;
           else
               keyLeft = true;
           //qDebug()<<"Key_Left pressed";
       }
       else if(event->key()==Qt::Key_Right)
       {
           if(event->isAutoRepeat())
               return ;
           else
               keyRight = true;
           //qDebug()<<"Key_Right pressed";
       }
       else
           return ;
}

void MyDialog::keyReleaseEvent(QKeyEvent *event)
{
    if(!okPressKey)
        return ;
    //bool col0Caled,col1Caled,col2Caled,col3Caled;//列 0-3 有过计算标志
    bool isCaled = false;//是否有过计算标志，默认为false
    int xAddLoc,yAddLoc;
    bool is2;
    bool doAction = false;
    bool full = true;
    bool fail = true;
    if(event->key()==Qt::Key_Up)
    {
        if(event->isAutoRepeat())
            return ;
        else
        {
            keyUp = false;
            //qDebug()<<"Key_Up Released";
            //上按键按下处理程序。。。
            for(int y = 0;y!=4;++y)
            {
                isCaled = false;
                if(array[0][y]+array[1][y]+array[2][y]+array[3][y])//y列有数
                {
                    if(array[0][y])//(0,y)有数
                    {
                        if(array[1][y])//(1,y)有数
                        {
                            if(array[0][y]==array[1][y])//(0,y)与(1,y)相等
                            {
                                if(!isCaled)
                                {
                                    array[0][y]*=2;//array[0][y]+array[1][y]放入array[0][y]
                                    array[1][y] = 0;
                                    source+=array[0][y];//加分
                                    isCaled = true;//标记为true
                                    if(array[2][y])//(2,y)有数
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                    else//(2,y)没数
                                    {
                                        array[1][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                    //qDebug()<<array[0][y];
                                }
                            }
                            else//(0,y)与(1,y)不相等
                            {
                                if(array[2][y])//(2,y)有数
                                {
                                    if(array[1][y]==array[2][y])//(1,y)与(2,y)相等
                                    {
                                        if(!isCaled)
                                        {
                                            array[1][y]*=2;
                                            source+=array[1][y];
                                            isCaled = true;
                                            doAction = true;
                                        }
                                    }
                                    else//(1,y)与(2,y)不相等
                                    {
                                        if(array[3][y])//(3,y)有数
                                        {
                                            if(array[2][y]==array[3][y])//(2,y)与(3,y)相等
                                            {
                                                if(!isCaled)
                                                {
                                                    array[2][y]*=2;
                                                    source+=array[2][y];
                                                    isCaled = true;
                                                    doAction = true;
                                                }
                                            }
                                            else//(2,y) != (3,y)
                                            {
                                                ;
                                            }
                                        }
                                        else//(3,y)没数
                                        {
                                            ;
                                        }
                                    }
                                }
                                else//(2,y)没数
                                {
                                    if(array[3][y])//(3,y)有数
                                    {
                                        if(array[1][y]==array[3][y])//(1,y) == (3,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[1][y]*=2;
                                                source+=array[1][y];
                                                array[3][y] = 0;
                                                isCaled = true;
                                                doAction = true;
                                            }
                                        }
                                        else//(1,y) != (3,y)
                                        {
                                            array[2][y] = array[3][y];
                                            array[3][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(3,y)没数
                                    {
                                        ;
                                    }
                                }
                            }
                        }
                        else//(1,y)没数
                        {
                            if(array[2][y])//(2,y)有数
                            {
                                if(array[0][y]==array[2][y])//(0,y) == (2,y)
                                {
                                    if(!isCaled)
                                    {
                                        array[0][y]*=2;
                                        source+=array[0][y];
                                        array[2][y] = 0;
                                        isCaled = true;
                                        array[1][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                {
                                    if(array[3][y])//(3,y)有数
                                    {
                                        if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[2][y]*=2;
                                                source+=array[2][y];
                                                array[3][y] =0;
                                                isCaled = true;
                                                array[1][y] = array[2][y];
                                                array[2][y] = 0;
                                                doAction = true;
                                            }
                                        }
                                        else
                                        {
                                            array[1][y] = array[2][y];
                                            array[2][y] = array[3][y];
                                            array[3][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(3,y)没数
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                                /*if(array[3][y])//(3,y)有数
                                {
                                    if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source+=array[2][y];
                                            array[3][y] = 0;
                                            isCaled = true;
                                            array[1][y] = array[2][y];
                                            array[2][y] = 0;
                                        }
                                    }
                                    else//(2,y) != (3,y)
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = array[3][y];
                                        array[3][y] = 0;
                                    }
                                }*/
                                /*else//(3,y)没数
                                {
                                    array[1][y] = array[2][y];
                                    array[2][y] = 0;
                                }
                            }*/
                            else//(2,y)没数
                            {
                                if(array[3][y])//(3,y)有数
                                {
                                    if(array[0][y]==array[3][y])
                                    {
                                        if(!isCaled)
                                        {
                                            array[0][y]*=2;
                                            source+=array[0][y];
                                            array[3][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else
                                    {
                                        array[1][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                    ;
                            }
                        }
                    }
                    else//(0,y)没数
                    {
                        if(array[1][y])//(1,y)有数
                        {
                            if(array[2][y])//(2,y)有数
                            {
                                if(array[1][y]==array[2][y])//(1,y) == (2,y)
                                {
                                    if(!isCaled)
                                    {
                                        array[1][y]*=2;
                                        source+=array[1][y];
                                        array[2][y] = 0;
                                        isCaled = true;
                                        array[0][y] = array[1][y];
                                        array[1][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(1,y) != (2,y)
                                {
                                    if(array[3][y])//(3,y)有数
                                    {
                                        if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[2][y]*=2;
                                                source+=array[2][y];
                                                array[3][y] = 0;
                                                isCaled = true;
                                                array[0][y] = array[1][y];
                                                array[1][y] = array[2][y];
                                                doAction = true;
                                            }
                                        }
                                        else//(2,y) != (3,y)
                                        {
                                            array[0][y] = array[1][y];
                                            array[1][y] = array[2][y];
                                            array[2][y] = array[3][y];
                                            array[3][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(3,y)没数
                                    {
                                        array[0][y] = array[1][y];
                                        array[1][y] = array[2][y];
                                        array[2][y] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                            else//(2,y)没数
                            {
                                if(array[3][y])//(3,y)有数
                                {
                                    if(array[1][y]==array[3][y])//(1,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[1][y]*=2;
                                            source = array[1][y];
                                            array[3][y] = 0;
                                            array[0][y] = array[1][y];
                                            array[1][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(1,y) != (3,y)
                                    {
                                        array[0][y] = array[1][y];
                                        array[1][y] = array[3][y];
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(3,y)没数
                                {
                                    array[0][y] = array[1][y];
                                    array[1][y] = 0;
                                    doAction = true;
                                }
                            }
                        }
                        else//(1,y)没数
                        {
                            if(array[2][y])//(2,y)有数
                            {
                                if(array[3][y])//(3,y)有数
                                {
                                    if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                          array[2][y]*=2;
                                          source+=array[2][y];
                                          array[3][y] = 0;
                                          array[0][y] = array[2][y];
                                          array[2][y] = 0;
                                          isCaled = true;
                                          doAction = true;
                                        }
                                    }
                                    else//(2,y) != (3,y)
                                    {
                                        array[0][y] = array[2][y];
                                        array[1][y] = array[3][y];
                                        array[2][y] = 0;
                                        array[3][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(3,y)没数
                                {
                                    array[0][y] = array[2][y];
                                    array[2][y] = 0;
                                    doAction = true;
                                }
                            }
                            else//(2,y)没数
                            {
                                array[0][y] = array[3][y];
                                array[3][y] = 0;
                                doAction = true;
                            }
                        }
                    }
                }
                isCaled = false;
            }
        }
        for(int i = 0;i!=4;++i)
            for(int j = 0;j!=4;++j)
                if(array[i][j]==0)
                    full = false;
        //在最后一行添加一个新数。。。
        //随机生成一个坐标
        if(doAction&&(!full))//有过操作，并且没有满，否则表示数字已经填满，不能再添加
        {
            do
            {
             xAddLoc = qrand()%4;
             yAddLoc = qrand()%4;
            }while(array[xAddLoc][yAddLoc]);
            is2 = qrand()%3;//当随机数是1或2时，is2为true,当随机数是0时，is2为false，所以有1/3的概率添加4，
                            //2/3的概率添加2
            if(is2)
                array[xAddLoc][yAddLoc]=2;
            else
                array[xAddLoc][yAddLoc]=4;
        }
    }
    else if(event->key()==Qt::Key_Down)
    {
        if(event->isAutoRepeat())
            return ;
        else
        {
            keyDown = false;
            //qDebug()<<"Key_Down Released";
            //下按键按下处理程序。。。
            for(int y = 0;y!=4;++y)
            {
                isCaled = false;
                if(array[3][y]+array[2][y]+array[1][y]+array[0][y])//y列有数
                {
                    if(array[3][y])//(3,y)有数
                    {
                        if(array[2][y])//(2,y)有数
                        {
                            if(array[3][y]==array[2][y])//(3,y)与(2,y)相等
                            {
                                if(!isCaled)
                                {
                                    array[3][y]*=2;//array[3][y]+array[2][y]放入array[3][y]
                                    array[2][y] = 0;
                                    source+=array[3][y];//加分
                                    isCaled = true;//标记为true
                                    if(array[1][y])//(1,y)有数
                                    {
                                        array[2][y] = array[1][y];
                                        array[1][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                    else//(1,y)没数
                                    {
                                        array[2][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                    //qDebug()<<array[0][y];
                                }
                            }
                            else//(3,y)与(2,y)不相等
                            {
                                if(array[1][y])//(1,y)有数
                                {
                                    if(array[2][y]==array[1][y])//(2,y)与(1,y)相等
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source+=array[2][y];
                                            isCaled = true;
                                            doAction = true;
                                        }
                                    }
                                    else//(2,y)与(1,y)不相等
                                    {
                                        if(array[0][y])//(0,y)有数
                                        {
                                            if(array[1][y]==array[0][y])//(1,y)与(0,y)相等
                                            {
                                                if(!isCaled)
                                                {
                                                    array[1][y]*=2;
                                                    source+=array[1][y];
                                                    isCaled = true;
                                                    doAction = true;
                                                }
                                            }
                                            else//(1,y) != (0,y)
                                            {
                                                ;
                                            }
                                        }
                                        else//(0,y)没数
                                        {
                                            ;
                                        }
                                    }
                                }
                                else//(1,y)没数
                                {
                                    if(array[0][y])//(0,y)有数
                                    {
                                        if(array[2][y]==array[0][y])//(2,y) == (0,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[2][y]*=2;
                                                source+=array[2][y];
                                                array[0][y] = 0;
                                                isCaled = true;
                                                doAction = true;
                                            }
                                        }
                                        else//(2,y) != (0,y)
                                        {
                                            array[1][y] = array[0][y];
                                            array[0][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(0,y)没数
                                    {
                                        ;
                                    }
                                }
                            }
                        }
                        else//(2,y)没数
                        {
                            if(array[1][y])//(1,y)有数
                            {
                                if(array[3][y]==array[1][y])//(0,y) == (1,y)
                                {
                                    if(!isCaled)
                                    {
                                        array[3][y]*=2;
                                        source+=array[3][y];
                                        array[1][y] = 0;
                                        isCaled = true;
                                        array[2][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                {
                                    if(array[0][y])//(0,y)有数
                                    {
                                        if(array[1][y]==array[0][y])//(1,y) == (0,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[1][y]*=2;
                                                source+=array[2][y];
                                                array[0][y] =0;
                                                isCaled = true;
                                                array[2][y] = array[1][y];
                                                array[1][y] = 0;
                                                doAction = true;
                                            }
                                        }
                                        else
                                        {
                                            array[2][y] = array[1][y];
                                            array[1][y] = array[0][y];
                                            array[0][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(0,y)没数
                                    {
                                        array[2][y] = array[1][y];
                                        array[1][y] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                                /*if(array[3][y])//(3,y)有数
                                {
                                    if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source+=array[2][y];
                                            array[3][y] = 0;
                                            isCaled = true;
                                            array[1][y] = array[2][y];
                                            array[2][y] = 0;
                                        }
                                    }
                                    else//(2,y) != (3,y)
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = array[3][y];
                                        array[3][y] = 0;
                                    }
                                }*/
                                /*else//(3,y)没数
                                {
                                    array[1][y] = array[2][y];
                                    array[2][y] = 0;
                                }
                            }*/
                            else//(1,y)没数
                            {
                                if(array[0][y])//(0,y)有数
                                {
                                    if(array[3][y]==array[0][y])
                                    {
                                        if(!isCaled)
                                        {
                                            array[3][y]*=2;
                                            source+=array[3][y];
                                            array[0][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else
                                    {
                                        array[2][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                    ;
                            }
                        }
                    }
                    else//(3,y)没数
                    {
                        if(array[2][y])//(2,y)有数
                        {
                            if(array[1][y])//(1,y)有数
                            {
                                if(array[2][y]==array[1][y])//(2,y) == (1,y)
                                {
                                    if(!isCaled)
                                    {
                                        array[2][y]*=2;
                                        source+=array[2][y];
                                        array[1][y] = 0;
                                        isCaled = true;
                                        array[3][y] = array[2][y];
                                        array[2][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(2,y) != (1,y)
                                {
                                    if(array[0][y])//(0,y)有数
                                    {
                                        if(array[1][y]==array[0][y])//(1,y) == (0,y)
                                        {
                                            if(!isCaled)
                                            {
                                                array[1][y]*=2;
                                                source+=array[1][y];
                                                array[0][y] = 0;
                                                isCaled = true;
                                                array[3][y] = array[2][y];
                                                array[2][y] = array[1][y];
                                                doAction = true;
                                            }
                                        }
                                        else//(1,y) != (0,y)
                                        {
                                            array[3][y] = array[2][y];
                                            array[2][y] = array[1][y];
                                            array[1][y] = array[0][y];
                                            array[0][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(0,y)没数
                                    {
                                        array[3][y] = array[2][y];
                                        array[2][y] = array[1][y];
                                        array[1][y] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                            else//(1,y)没数
                            {
                                if(array[0][y])//(0,y)有数
                                {
                                    if(array[2][y]==array[0][y])//(2,y) == (0,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source = array[2][y];
                                            array[0][y] = 0;
                                            array[3][y] = array[2][y];
                                            array[2][y] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(2,y) != (0,y)
                                    {
                                        array[3][y] = array[2][y];
                                        array[2][y] = array[0][y];
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(0,y)没数
                                {
                                    array[3][y] = array[2][y];
                                    array[2][y] = 0;
                                    doAction = true;
                                }
                            }
                        }
                        else//(2,y)没数
                        {
                            if(array[1][y])//(1,y)有数
                            {
                                if(array[0][y])//(0,y)有数
                                {
                                    if(array[1][y]==array[0][y])//(1,y) == (0,y)
                                    {
                                        if(!isCaled)
                                        {
                                          array[1][y]*=2;
                                          source+=array[1][y];
                                          array[0][y] = 0;
                                          array[3][y] = array[1][y];
                                          array[1][y] = 0;
                                          isCaled = true;
                                          doAction = true;
                                        }
                                    }
                                    else//(1,y) != (0,y)
                                    {
                                        array[3][y] = array[1][y];
                                        array[2][y] = array[0][y];
                                        array[1][y] = 0;
                                        array[0][y] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(0,y)没数
                                {
                                    array[3][y] = array[1][y];
                                    array[1][y] = 0;
                                    doAction = true;
                                }
                            }
                            else//(1,y)没数
                            {
                                array[3][y] = array[0][y];
                                array[0][y] = 0;
                                doAction = true;
                            }
                        }
                    }
                }
                isCaled = false;
            }
        }
        for(int i = 0;i!=4;++i)
            for(int j = 0;j!=4;++j)
                if(array[i][j]==0)
                    full = false;
        //在最后一行添加一个新数。。。
        //随机生成一个坐标
        if(!full)//没有填满，可以再添加
        {
            do
            {
             xAddLoc = qrand()%4;
             yAddLoc = qrand()%4;
            }while(array[xAddLoc][yAddLoc]);
            is2 = qrand()%3;//当随机数是1或2时，is2为true,当随机数是0时，is2为false，所以有1/3的概率添加4，
                            //2/3的概率添加2
            if(is2)
                array[xAddLoc][yAddLoc]=2;
            else
                array[xAddLoc][yAddLoc]=4;
        }
    }
    else if(event->key()==Qt::Key_Left)
    {
        if(event->isAutoRepeat())
            return ;
        else
        {
            keyLeft = false;
            //qDebug()<<"Key_Left Released";
            //左键按下处理程序。。。
            for(int x = 0;x!=4;++x)
            {
                isCaled = false;
                if(array[x][0]+array[x][1]+array[x][2]+array[x][3])//y列有数
                {
                    if(array[x][0])//(x,0)有数
                    {
                        if(array[x][1])//(x,1)有数
                        {
                            if(array[x][0]==array[x][1])//(x,0)与(x,1)相等
                            {
                                if(!isCaled)
                                {
                                    array[x][0]*=2;//array[x][0]+array[x][1]放入array[x][0]
                                    array[x][1] = 0;
                                    source+=array[x][0];//加分
                                    isCaled = true;//标记为true
                                    if(array[x][2])//(x,2)有数
                                    {
                                        array[x][1] = array[x][2];
                                        array[x][2] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                    else//(x,2)没数
                                    {
                                        array[x][1] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                    //qDebug()<<array[0][y];
                                }
                            }
                            else//(x,0)与(x,1)不相等
                            {
                                if(array[x][2])//(x,2)有数
                                {
                                    if(array[x][1]==array[x][2])//(x,1)与(x,2)相等
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][1]*=2;
                                            source+=array[x][1];
                                            isCaled = true;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,1)与(x,2)不相等
                                    {
                                        if(array[x][3])//(x,3)有数
                                        {
                                            if(array[x][2]==array[x][3])//(x,2)与(x,3)相等
                                            {
                                                if(!isCaled)
                                                {
                                                    array[x][2]*=2;
                                                    source+=array[x][2];
                                                    isCaled = true;
                                                    doAction = true;
                                                }
                                            }
                                            else//(x,2) != (x,3)
                                            {
                                                ;
                                            }
                                        }
                                        else//(x,3)没数
                                        {
                                            ;
                                        }
                                    }
                                }
                                else//(x,2)没数
                                {
                                    if(array[x][3])//(x,3)有数
                                    {
                                        if(array[x][1]==array[x][3])//(x,1) == (x,3)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][1]*=2;
                                                source+=array[x][1];
                                                array[x][3] = 0;
                                                isCaled = true;
                                                doAction = true;
                                            }
                                        }
                                        else//(x,1) != (x,3)
                                        {
                                            array[x][2] = array[x][3];
                                            array[x][3] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,3)没数
                                    {
                                        ;
                                    }
                                }
                            }
                        }
                        else//(x,1)没数
                        {
                            if(array[x][2])//(x,2)有数
                            {
                                if(array[x][0]==array[x][2])//(x,0) == (x,2)
                                {
                                    if(!isCaled)
                                    {
                                        array[x][0]*=2;
                                        source+=array[x][0];
                                        array[x][2] = 0;
                                        isCaled = true;
                                        array[x][1] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                {
                                    if(array[x][3])//(x,3)有数
                                    {
                                        if(array[x][2]==array[x][3])//(x,2) == (x,3)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][2]*=2;
                                                source+=array[x][2];
                                                array[x][3] =0;
                                                isCaled = true;
                                                array[x][1] = array[x][2];
                                                array[x][2] = 0;
                                                doAction = true;
                                            }
                                        }
                                        else
                                        {
                                            array[x][1] = array[x][2];
                                            array[x][2] = array[x][3];
                                            array[x][3] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,3)没数
                                    {
                                        array[x][1] = array[x][2];
                                        array[x][2] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                                /*if(array[3][y])//(3,y)有数
                                {
                                    if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source+=array[2][y];
                                            array[3][y] = 0;
                                            isCaled = true;
                                            array[1][y] = array[2][y];
                                            array[2][y] = 0;
                                        }
                                    }
                                    else//(2,y) != (3,y)
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = array[3][y];
                                        array[3][y] = 0;
                                    }
                                }*/
                                /*else//(3,y)没数
                                {
                                    array[1][y] = array[2][y];
                                    array[2][y] = 0;
                                }
                            }*/
                            else//(x,2)没数
                            {
                                if(array[x][3])//(x,3)有数
                                {
                                    if(array[x][0]==array[x][3])
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][0]*=2;
                                            source+=array[x][0];
                                            array[x][3] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else
                                    {
                                        array[x][1] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                    ;
                            }
                        }
                    }
                    else//(x,0)没数
                    {
                        if(array[x][1])//(x,1)有数
                        {
                            if(array[x][2])//(x,2)有数
                            {
                                if(array[x][1]==array[x][2])//(x,1) == (x,2)
                                {
                                    if(!isCaled)
                                    {
                                        array[x][1]*=2;
                                        source+=array[x][1];
                                        array[x][2] = 0;
                                        isCaled = true;
                                        array[x][0] = array[x][1];
                                        array[x][1] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,1) != (x,2)
                                {
                                    if(array[x][3])//(x,3)有数
                                    {
                                        if(array[x][2]==array[x][3])//(x,2) == (x,3)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][2]*=2;
                                                source+=array[x][2];
                                                array[x][3] = 0;
                                                isCaled = true;
                                                array[x][0] = array[x][1];
                                                array[x][1] = array[x][2];
                                                doAction = true;
                                            }
                                        }
                                        else//(x,2) != (x,3)
                                        {
                                            array[x][0] = array[x][1];
                                            array[x][1] = array[x][2];
                                            array[x][2] = array[x][3];
                                            array[x][3] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,3)没数
                                    {
                                        array[x][0]= array[x][1];
                                        array[x][1] = array[x][2];
                                        array[x][2] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                            else//(x,2)没数
                            {
                                if(array[x][3])//(x,3)有数
                                {
                                    if(array[x][1]==array[x][3])//(x,1) == (x,3)
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][1]*=2;
                                            source = array[x][1];
                                            array[x][3] = 0;
                                            array[x][0] = array[x][1];
                                            array[x][1] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,1) != (x,3)
                                    {
                                        array[x][0] = array[x][1];
                                        array[x][1] = array[x][3];
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,3)没数
                                {
                                    array[x][0] = array[x][1];
                                    array[x][1] = 0;
                                    doAction = true;
                                }
                            }
                        }
                        else//(x,1)没数
                        {
                            if(array[x][2])//(x,2)有数
                            {
                                if(array[x][3])//(x,3)有数
                                {
                                    if(array[x][2]==array[x][3])//(x,2) == (x,3)
                                    {
                                        if(!isCaled)
                                        {
                                          array[x][2]*=2;
                                          source+=array[x][2];
                                          array[x][3] = 0;
                                          array[x][0] = array[x][2];
                                          array[x][2] = 0;
                                          isCaled = true;
                                          doAction = true;
                                        }
                                    }
                                    else//(x,2) != (x,3)
                                    {
                                        array[x][0] = array[x][2];
                                        array[x][1] = array[x][3];
                                        array[x][2] = 0;
                                        array[x][3] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,3)没数
                                {
                                    array[x][0] = array[x][2];
                                    array[x][2] = 0;
                                    doAction = true;
                                }
                            }
                            else//(x,2)没数
                            {
                                array[x][0] = array[x][3];
                                array[x][3] = 0;
                                doAction = true;
                            }
                        }
                    }
                }
                isCaled = false;
            }
        }
        for(int i = 0;i!=4;++i)
            for(int j = 0;j!=4;++j)
                if(array[i][j]==0)
                    full = false;
        //在最后一行添加一个新数。。。
        //随机生成一个坐标
        if(doAction&&(!full))//有过操作，并且没有满，否则表示数字已经填满，不能再添加
        {
            do
            {
             xAddLoc = qrand()%4;
             yAddLoc = qrand()%4;
            }while(array[xAddLoc][yAddLoc]);
            is2 = qrand()%3;//当随机数是1或2时，is2为true,当随机数是0时，is2为false，所以有1/3的概率添加4，
                            //2/3的概率添加2
            if(is2)
                array[xAddLoc][yAddLoc]=2;
            else
                array[xAddLoc][yAddLoc]=4;
        }
    }
    else if(event->key()==Qt::Key_Right)
    {
        if(event->isAutoRepeat())
            return ;
        else
        {
            keyRight = false;
            //qDebug()<<"Key_Right Released";
            //右按键按下处理程序。。。
            for(int x = 0;x!=4;++x)
            {
                isCaled = false;
                if(array[x][3]+array[x][2]+array[x][1]+array[x][0])//x行有数
                {
                    if(array[x][3])//(x,3)有数
                    {
                        if(array[x][2])//(x,2)有数
                        {
                            if(array[x][3]==array[x][2])//(x,3)与(x,2)相等
                            {
                                if(!isCaled)
                                {
                                    array[x][3]*=2;//array[x][3]+array[x][2]放入array[x][3]
                                    array[x][2] = 0;
                                    source+=array[x][3];//加分
                                    isCaled = true;//标记为true
                                    if(array[x][1])//(x,1)有数
                                    {
                                        array[x][2] = array[x][1];
                                        array[x][1] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                    else//(x,1)没数
                                    {
                                        array[x][2] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                    //qDebug()<<array[0][y];
                                }
                            }
                            else//(x,3)与(x,2)不相等
                            {
                                if(array[x][1])//(x,1)有数
                                {
                                    if(array[x][2]==array[x][1])//(x,2)与(x,1)相等
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][2]*=2;
                                            source+=array[x][2];
                                            isCaled = true;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,2)与(x,1)不相等
                                    {
                                        if(array[x][0])//(x,0)有数
                                        {
                                            if(array[x][1]==array[x][0])//(x,1)与(x,0)相等
                                            {
                                                if(!isCaled)
                                                {
                                                    array[x][1]*=2;
                                                    source+=array[x][1];
                                                    isCaled = true;
                                                    doAction = true;
                                                }
                                            }
                                            else//(x,1) != (x,0)
                                            {
                                                ;
                                            }
                                        }
                                        else//(x,0)没数
                                        {
                                            ;
                                        }
                                    }
                                }
                                else//(x,1)没数
                                {
                                    if(array[x][0])//(x,0)有数
                                    {
                                        if(array[x][2]==array[x][0])//(x,2) == (x,0)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][2]*=2;
                                                source+=array[x][2];
                                                array[x][0] = 0;
                                                isCaled = true;
                                                doAction = true;
                                            }
                                        }
                                        else//(x,2) != (x,0)
                                        {
                                            array[x][1] = array[x][0];
                                            array[x][0] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,0)没数
                                    {
                                        ;
                                    }
                                }
                            }
                        }
                        else//(x,2)没数
                        {
                            if(array[x][1])//(x,1)有数
                            {
                                if(array[x][3]==array[x][1])//(x,0) == (x,1)
                                {
                                    if(!isCaled)
                                    {
                                        array[x][3]*=2;
                                        source+=array[x][3];
                                        array[x][1] = 0;
                                        isCaled = true;
                                        array[x][2] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                {
                                    if(array[x][0])//(x,0)有数
                                    {
                                        if(array[x][1]==array[x][0])//(x,1) == (x,0)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][1]*=2;
                                                source+=array[x][2];
                                                array[x][0] =0;
                                                isCaled = true;
                                                array[x][2] = array[x][1];
                                                array[x][1] = 0;
                                                doAction = true;
                                            }
                                        }
                                        else
                                        {
                                            array[x][2] = array[x][1];
                                            array[x][1] = array[x][0];
                                            array[x][0] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,0)没数
                                    {
                                        array[x][2] = array[x][1];
                                        array[x][1] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                                /*if(array[3][y])//(3,y)有数
                                {
                                    if(array[2][y]==array[3][y])//(2,y) == (3,y)
                                    {
                                        if(!isCaled)
                                        {
                                            array[2][y]*=2;
                                            source+=array[2][y];
                                            array[3][y] = 0;
                                            isCaled = true;
                                            array[1][y] = array[2][y];
                                            array[2][y] = 0;
                                        }
                                    }
                                    else//(2,y) != (3,y)
                                    {
                                        array[1][y] = array[2][y];
                                        array[2][y] = array[3][y];
                                        array[3][y] = 0;
                                    }
                                }*/
                                /*else//(3,y)没数
                                {
                                    array[1][y] = array[2][y];
                                    array[2][y] = 0;
                                }
                            }*/
                            else//(x,1)没数
                            {
                                if(array[x][0])//(x,0)有数
                                {
                                    if(array[x][3]==array[x][0])
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][3]*=2;
                                            source+=array[x][3];
                                            array[x][0] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else
                                    {
                                        array[x][2] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                }
                                else
                                    ;
                            }
                        }
                    }
                    else//(x,3)没数
                    {
                        if(array[x][2])//(x,2)有数
                        {
                            if(array[x][1])//(x,1)有数
                            {
                                if(array[x][2]==array[x][1])//(x,2) == (x,1)
                                {
                                    if(!isCaled)
                                    {
                                        array[x][2]*=2;
                                        source+=array[x][2];
                                        array[x][1] = 0;
                                        isCaled = true;
                                        array[x][3] = array[x][2];
                                        array[x][2] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,2) != (x,1)
                                {
                                    if(array[x][0])//(x,0)有数
                                    {
                                        if(array[x][1]==array[x][0])//(x,1) == (x,0)
                                        {
                                            if(!isCaled)
                                            {
                                                array[x][1]*=2;
                                                source+=array[x][1];
                                                array[x][0] = 0;
                                                isCaled = true;
                                                array[x][3] = array[x][2];
                                                array[x][2] = array[x][1];
                                                doAction = true;
                                            }
                                        }
                                        else//(x,1) != (x,0)
                                        {
                                            array[x][3] = array[x][2];
                                            array[x][2] = array[x][1];
                                            array[x][1] = array[x][0];
                                            array[x][0] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,0)没数
                                    {
                                        array[x][3] = array[x][2];
                                        array[x][2] = array[x][1];
                                        array[x][1] = 0;
                                        doAction = true;
                                    }
                                }
                            }
                            else//(x,1)没数
                            {
                                if(array[x][0])//(x,0)有数
                                {
                                    if(array[x][2]==array[x][0])//(x,2) == (x,0)
                                    {
                                        if(!isCaled)
                                        {
                                            array[x][2]*=2;
                                            source = array[x][2];
                                            array[x][0] = 0;
                                            array[x][3] = array[x][2];
                                            array[x][2] = 0;
                                            doAction = true;
                                        }
                                    }
                                    else//(x,2) != (x,0)
                                    {
                                        array[x][3] = array[x][2];
                                        array[x][2] = array[x][0];
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,0)没数
                                {
                                    array[x][3] = array[x][2];
                                    array[x][2] = 0;
                                    doAction = true;
                                }
                            }
                        }
                        else//(x,2)没数
                        {
                            if(array[x][1])//(x,1)有数
                            {
                                if(array[x][0])//(x,0)有数
                                {
                                    if(array[x][1]==array[x][0])//(x,1) == (x,0)
                                    {
                                        if(!isCaled)
                                        {
                                          array[x][1]*=2;
                                          source+=array[x][1];
                                          array[x][0] = 0;
                                          array[x][3] = array[x][1];
                                          array[x][1] = 0;
                                          isCaled = true;
                                          doAction = true;
                                        }
                                    }
                                    else//(x,1) != (x,0)
                                    {
                                        array[x][3] = array[x][1];
                                        array[x][2] = array[x][0];
                                        array[x][1] = 0;
                                        array[x][0] = 0;
                                        doAction = true;
                                    }
                                }
                                else//(x,0)没数
                                {
                                    array[x][3] = array[x][1];
                                    array[x][1] = 0;
                                    doAction = true;
                                }
                            }
                            else//(x,1)没数
                            {
                                array[x][3] = array[x][0];
                                array[x][0] = 0;
                                doAction = true;
                            }
                        }
                    }
                }
                isCaled = false;
            }
        }
        for(int i = 0;i!=4;++i)
            for(int j = 0;j!=4;++j)
                if(array[i][j]==0)
                    full = false;
        //在最后一行添加一个新数。。。
        //随机生成一个坐标
        if(!full)//没有填满，可以再添加
        {
            do
            {
             xAddLoc = qrand()%4;
             yAddLoc = qrand()%4;
            }while(array[xAddLoc][yAddLoc]);
            is2 = qrand()%3;//当随机数是1或2时，is2为true,当随机数是0时，is2为false，所以有1/3的概率添加4，
                            //2/3的概率添加2
            if(is2)
                array[xAddLoc][yAddLoc]=2;
            else
                array[xAddLoc][yAddLoc]=4;
        }
    }
    else
        return ;
    //显示新数字。。。
    /*for(int i=0;i!=4;++i)
            for(int j=0;j!=4;++j)
                qDebug()<<array[i][j];*/
    ui->label00->setText(QString::number(array[0][0])=="0"?" ":QString::number(array[0][0]));
    ui->label01->setText(QString::number(array[0][1])=="0"?" ":QString::number(array[0][1]));
    ui->label02->setText(QString::number(array[0][2])=="0"?" ":QString::number(array[0][2]));
    ui->label03->setText(QString::number(array[0][3])=="0"?" ":QString::number(array[0][3]));
    ui->label10->setText(QString::number(array[1][0])=="0"?" ":QString::number(array[1][0]));
    ui->label11->setText(QString::number(array[1][1])=="0"?" ":QString::number(array[1][1]));
    ui->label12->setText(QString::number(array[1][2])=="0"?" ":QString::number(array[1][2]));
    ui->label13->setText(QString::number(array[1][3])=="0"?" ":QString::number(array[1][3]));
    ui->label20->setText(QString::number(array[2][0])=="0"?" ":QString::number(array[2][0]));
    ui->label21->setText(QString::number(array[2][1])=="0"?" ":QString::number(array[2][1]));
    ui->label22->setText(QString::number(array[2][2])=="0"?" ":QString::number(array[2][2]));
    ui->label23->setText(QString::number(array[2][3])=="0"?" ":QString::number(array[2][3]));
    ui->label30->setText(QString::number(array[3][0])=="0"?" ":QString::number(array[3][0]));
    ui->label31->setText(QString::number(array[3][1])=="0"?" ":QString::number(array[3][1]));
    ui->label32->setText(QString::number(array[3][2])=="0"?" ":QString::number(array[3][2]));
    ui->label33->setText(QString::number(array[3][3])=="0"?" ":QString::number(array[3][3]));
    ui->sourcePrint->setText(QString::number(source));
    //判断游戏是否结束
    fail = true;
    for(int i = 0;i!=4;++i)
    {
        for(int j = 0;j!=4;++j)
        {
            if(j==3)
            {
                if(array[i][j]==array[i+1][j])
                    fail = false;
            }
            if(i==3)
            {
                if(array[i][j]==array[i][j+1])
                    fail = false;
            }
            if(i<3&&j<3)
            {
                if(array[i][j]==array[i+1][j]||array[i][j]==array[i][j+1])
                    fail = false;
            }
        }
    }
    if(fail&&full)//游戏结束
    {
        /*ui->label00->setText(" ");
        ui->label01->setText(" ");
        ui->label02->setText(" ");
        ui->label03->setText(" ");
        ui->label10->setText("L");
        ui->label11->setText("O");
        ui->label12->setText("S");
        ui->label13->setText("E");
        ui->label20->setText("L");
        ui->label21->setText("O");
        ui->label22->setText("S");
        ui->label23->setText("E");
        ui->label30->setText(" ");
        ui->label31->setText(" ");
        ui->label32->setText(" ");
        ui->label33->setText(" ");*/
        ui->lossLabel->setText("LOSS");
        okPressKey = false;
        //debug
        /*for(int i = 0;i!=4;++i)
            for(int j = 0;j!=4;++j)
                qDebug()<<array[i][j];*/
    }
}

void MyDialog::on_startButton_clicked()//点击开始游戏键
{
    //start game
    int xf1,xf2,xf3,yf1,yf2,yf3;//初始三个坐标
    //MyDialog *dlg = new MyDialog;
    QString string;
    xf1 = xf2 = xf3 = yf1 = yf2 = yf3 = 0;
    for(int i = 0;i!=4;++i)
        for(int j = 0;j!= 4;++j)
            array[i][j] = 0;
    source = 0;
    ui->lossLabel->setText(" ");
    while(((xf1==xf2)&&(yf1==yf2))||((xf1==xf3)&&(yf1==yf3))||((xf2==xf3)&&(yf2==yf3)))//坐标相同则再次随机
    {
        xf1 = qrand()%4;
        xf2 = qrand()%4;
        xf3 = qrand()%4;
        yf1 = qrand()%4;
        yf2 = qrand()%4;
        yf3 = qrand()%4;
        //debug
        /*xf1 = 0;
        xf2 = 1;
        xf3 = 3;
        yf1 = 0;
        yf2 = 0;
        yf3 = 0;*/
    }
    /*qDebug()<<"xf1 = "<<xf1<<endl
              <<"xf2 = "<<xf2<<endl
                <<"xf3 = "<<xf3<<endl
                  <<"yf1 = "<<yf1<<endl
                    <<"yf2 = "<<yf2<<endl
                       <<"yf3 = "<<yf3<<endl;*/
    //设置初始值
    array[xf1][yf1] = 2;
    array[xf2][yf2] = 2;
    array[xf3][yf3] = 2;
    /*for(int i=0;i!=4;++i)
        for(int j=0;j!=4;++j)
            qDebug()<<array[i][j];*/
    //初始化显示
    ui->label00->setText(QString::number(array[0][0])=="0"?" ":QString::number(array[0][0]));
    ui->label01->setText(QString::number(array[0][1])=="0"?" ":QString::number(array[0][1]));
    ui->label02->setText(QString::number(array[0][2])=="0"?" ":QString::number(array[0][2]));
    ui->label03->setText(QString::number(array[0][3])=="0"?" ":QString::number(array[0][3]));
    ui->label10->setText(QString::number(array[1][0])=="0"?" ":QString::number(array[1][0]));
    ui->label11->setText(QString::number(array[1][1])=="0"?" ":QString::number(array[1][1]));
    ui->label12->setText(QString::number(array[1][2])=="0"?" ":QString::number(array[1][2]));
    ui->label13->setText(QString::number(array[1][3])=="0"?" ":QString::number(array[1][3]));
    ui->label20->setText(QString::number(array[2][0])=="0"?" ":QString::number(array[2][0]));
    ui->label21->setText(QString::number(array[2][1])=="0"?" ":QString::number(array[2][1]));
    ui->label22->setText(QString::number(array[2][2])=="0"?" ":QString::number(array[2][2]));
    ui->label23->setText(QString::number(array[2][3])=="0"?" ":QString::number(array[2][3]));
    ui->label30->setText(QString::number(array[3][0])=="0"?" ":QString::number(array[3][0]));
    ui->label31->setText(QString::number(array[3][1])=="0"?" ":QString::number(array[3][1]));
    ui->label32->setText(QString::number(array[3][2])=="0"?" ":QString::number(array[3][2]));
    ui->label33->setText(QString::number(array[3][3])=="0"?" ":QString::number(array[3][3]));
    ui->sourcePrint->setText("0");
    okPressKey = true;
}

void MyDialog::on_aboutButton_clicked()
{
    aboutDialog *abdlg = new aboutDialog(this);
    abdlg->show();
}

void MyDialog::on_helpButton_clicked()
{
    helpDialog *help = new helpDialog(this);
    help->show();
}
