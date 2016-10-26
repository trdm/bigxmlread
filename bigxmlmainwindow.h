#ifndef MAINWINDOWBIGXMLREADER_H
#define MAINWINDOWBIGXMLREADER_H

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QListWidget>
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
    void reateDockWindows();
public slots:
    void open();
    void find();
    void propertyCurFile();
    void findNext();
    void findPrevious();
    void about();
    void changeCurPath(QString& txt);
    void openRecentFile();
protected:
    void closeEvent( QCloseEvent *ev);
private:
    void createActions();
    void createMenus();
    void updateRecentFileActions();
    void createDockWindows();
    QString strippedName(const QString &fullFileName);

    BigXmlReader bigxmlWidget;

    QString m_curFileName;
    QListWidget *m_mesages;
    QDockWidget *m_mesageDock;

    QMenu *fileMenu;
    QMenu *findMenu;
    QMenu *recentFilesMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *propertyAct;
    QAction *toggleViewMessagesAct;
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
