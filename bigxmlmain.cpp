#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include "bigxmlmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("sikuda");
    app.setApplicationName("bigxmlread");//    sikuda/bigxmlread

    MainWindow mainWin;
    mainWin.show();
    QString tmpLine = "";
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("IBM866"));
    QString pathFname = "";
    bool pathFn = false;
    for (int i = 1; i<argc; i++ )
    {
        tmpLine = QString::fromLocal8Bit(argv[i]);
        qDebug() << tmpLine;
        pathFname = tmpLine; //.fromLocal8Bit();
        if (QFile::exists(pathFname)){
            pathFn = true;
            break;
        }
    }
    if (pathFn){
        mainWin.openFile(pathFname);
    } else {
        if (mainWin.getCountRecentFiles() == 0)
            mainWin.open();
    }
    return app.exec();
}
