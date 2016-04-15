#ifndef MAINWINDOWBIGXMLREADER_H
#define MAINWINDOWBIGXMLREADER_H

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include "bigxmlreader.h"

//QT_BEGIN_NAMESPACE
//    class BigXmlReader;
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void openFile(QString& fileName);
    int getCountRecentFiles() {return m_countRecentFiles; }
public slots:
    void open();
    void find();
    void findNext();
    void findPrevious();
    void about();
    void changeCurPath(QString& txt);
    void openRecentFile();

private:
    void createActions();
    void createMenus();
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    BigXmlReader bigxmlWidget;

    QString curFile;

    QMenu *fileMenu;
    QMenu *findMenu;
    QMenu *recentFilesMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *findAct;
    QAction *findActNext;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *separatorAct;
    QToolBar *tb;
    QComboBox* m_cBoxCurPath;
    enum { MaxRecentFiles = 20 };
    QAction *recentFileActs[MaxRecentFiles];

    int m_countRecentFiles;

};

#endif
