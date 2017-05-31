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
    void findBy();
    void findNext();
    void propertyCurFile();
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
    void updateFindString();
    void showNewMessage(QString messageStr);

    QString strippedName(const QString &fullFileName);
    int m_maxSizeFileFullOpen;

    BigXmlReader bigxmlWidget;

    QString m_curFileName;
    QString m_curentPath;
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
    QComboBox* m_cBoxSearching;
    enum { MaxRecentFiles = 20 };
    QAction *recentFileActs[MaxRecentFiles];
    qint64 m_openFileSize;
    qint64 m_1MbToBute;
    int m_countRecentFiles;

};

#endif
