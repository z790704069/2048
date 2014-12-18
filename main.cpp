/*
*Copyright（C） 2014.4 Daivid Charles
*这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证条款来修改和重新发布这一程序。
*或者用许可证的第二版，或者（根据你的选择）用任何更新的版本。
*This program is free software, and you can follow the Free Software Foundation's GNU General Public License
*as published by the terms to modify and redistribute the program. Or a second license, or (at your option)
*any later version.
*/
#include <QApplication>
#include <QTextCodec>
#include "mydialog.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    MyDialog w;
    w.show();

    /*QFile qssFile("./style.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        QString qss = QLatin1String(qssFile.readAll());
        app.setStyleSheet(qss);
        qssFile.close();
    }*/
    return app.exec();
}
