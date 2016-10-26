#include <QtGui>
#include <QSettings>

#include "bigxmlmainwindow.h"
#include "bigxmlreader.h"

MainWindow::MainWindow()
{    
    QStringList labels;
    labels << tr("Node/Attribute")  << tr("Value/Comment"); //<< tr("Child count") ;
    m_mesages = 0;
    m_mesageDock = 0;

    bigxmlWidget.header()->setResizeMode(QHeaderView::ResizeToContents);
    bigxmlWidget.header()->setResizeMode(QHeaderView::Interactive);
    bigxmlWidget.setHeaderLabels(labels);
    setCentralWidget(&bigxmlWidget);
    bigxmlWidget.setColumnWidth(0,250);
    bigxmlWidget.setColumnWidth(1,400);
    bigxmlWidget.setUniformRowHeights(true);

    createActions();
    createMenus();
    createDockWindows();

    statusBar()->showMessage(tr("Ready"));
    connect(&bigxmlWidget,SIGNAL(changeCurPath(QString&)), this,SLOT(changeCurPath(QString&)));
    //void changeCurPath(QString& txt);
    setWindowTitle(tr("BigXmlReader trdm Edition"));
    QSettings set("trdm","bigxmlreader");
    int ww = set.value("width",620).toInt();
    int wh = set.value("height",680).toInt();
    ww = qMax(ww, 620);
    wh = qMax(wh, 680);
    //qDebug() << "ww"<<ww<<"wh"<<wh;
    resize(ww,wh);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
        return;
    m_curFileName = fileName;
    openFile(fileName);

}

void MainWindow::find()
{
    QLabel* label = new QLabel(tr("Find:"));
    QLineEdit* lineEdit = new QLineEdit;
    lineEdit->setText(bigxmlWidget.strFindString);
    label->setBuddy(lineEdit);

    QPushButton* findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);

    QHBoxLayout *Layout = new QHBoxLayout;
    Layout->addWidget(label);
    Layout->addWidget(lineEdit);
    Layout->addWidget(findButton);

    QDialog findDialog(this);
    findDialog.setMinimumSize(320, 60);
    findDialog.setLayout(Layout);
    findDialog.setWindowTitle(tr("BigXmlReader"));
    connect(findButton, SIGNAL(pressed()), &findDialog, SLOT(accept()));

    if( findDialog.exec() == QDialog::Accepted){
        QXmlStreamReader xml;
        bigxmlWidget.strFindString = lineEdit->text();
        QString strFilename = bigxmlWidget.getFilename();
        if( bigxmlWidget.openFile(strFilename, xml, false)){
            QString findString = lineEdit->text();
            if( bigxmlWidget.findDataBigXML( xml, findString, 0 )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
            else QMessageBox::warning(this, tr("BigXmlReader"), tr("Can't find %1").arg(findString));
        }
        findDialog.close();
    }
}

void MainWindow::propertyCurFile()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr(" BigXmlReader - open curent file name "),
                                             tr("Curent file name:"), QLineEdit::Normal,
                                             m_curFileName, &ok);

}

void MainWindow::findNext()
{
    QXmlStreamReader xml;
    QTreeWidgetItem* item = bigxmlWidget.currentItem();
    if( bigxmlWidget.openFile(bigxmlWidget.strFindString, xml, false)){
        QString findString = bigxmlWidget.strFindString;
        if( bigxmlWidget.findDataBigXML( xml, findString, item )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
        else QMessageBox::warning(this, tr("BigXmlReader"), tr("Can't find %1").arg(findString));
    }
}

void MainWindow::findPrevious()
{
    // TODO
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About BigXmlReader"),
                      QString(tr(
                         "The <b>BigXmlReader</b> example demonstrates how to read big xml file.\n\r"
                         "<br>"                                  
                         "E-mail: sikuda@yandex.ru"
                         "<a href = \"https://github.com/sikuda/bigxmlread\">https://github.com/sikuda/bigxmlread</a>"
                         "<br>"
                         "<br>"
                         "Edition of trdm E-mail: trdmval@gmail.com"
                         "<br>"
                         "<a href = \"https://github.com/trdm/bigxmlread\">https://github.com/trdm/bigxmlread</a>"
                         "<br>"
                         "Compiled at %1 %2"
                         )).arg(__DATE__).arg(__TIME__));
}

void MainWindow::changeCurPath(QString &txt)
{
    int pos = m_cBoxCurPath->findText(txt);
    if (pos == -1){
        m_cBoxCurPath->addItem(txt);
        pos = m_cBoxCurPath->findText(txt);
    }
    m_cBoxCurPath->setCurrentIndex(pos);
}

void MainWindow::createActions()
{
    propertyAct = new QAction(tr("&Property..."), this);
    connect(propertyAct, SIGNAL(triggered()), this, SLOT(propertyCurFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    findAct = new QAction(tr("&Find..."), this);
    findAct->setShortcuts(QKeySequence::Find);
    connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

    findActNext = new QAction(tr("F&ind next..."), this);
    findActNext->setShortcuts(QKeySequence::FindNext);
    connect(findActNext, SIGNAL(triggered()), this, SLOT(findNext()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

//    findActPrevious = new QAction(tr("F&ind next..."), this);
//    findActPrevious->setShortcuts(QKeySequence::FindPrevious);
//    connect(findActPrevious, SIGNAL(triggered()), this, SLOT(findPrevious()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(propertyAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
         fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    //menuBar()->addSeparator();
    updateRecentFileActions();

    findMenu = menuBar()->addMenu(tr("Find"));
    findMenu->addAction(findAct);
    findMenu->addAction(findActNext);
    //findMenu->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("File Actions"));
    addToolBar(tb);

    QLabel* label = new QLabel(this);
    label->setText("Cur path:");
    tb->addWidget(label);

    m_cBoxCurPath = new QComboBox(this);
    tb->addWidget(m_cBoxCurPath);
    m_cBoxCurPath->setEditable(true);
    m_cBoxCurPath->setMaximumWidth(200);
    m_cBoxCurPath->setMinimumWidth(200);

}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    m_countRecentFiles = numRecentFiles;
    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

void MainWindow::createDockWindows()
{
    m_mesageDock = new QDockWidget(tr("Messages"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_mesages = new QListWidget(m_mesageDock); //customerList ->m_mesages

    m_mesageDock->setWidget(m_mesages);
    addDockWidget(Qt::BottomDockWidgetArea, m_mesageDock);
    toggleViewMessagesAct = m_mesageDock->toggleViewAction();
    viewMenu->addAction(toggleViewMessagesAct);
    toggleViewMessagesAct->setChecked(false);
    m_mesageDock->hide();

}

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openFile(QString &fileName)
{
    QXmlStreamReader xml;
    m_curFileName = fileName;
    if( bigxmlWidget.openFile(fileName, xml)){
        //if (!bigxmlWidget.readBigXML(xml)) {
        // Было if (!bigxmlWidget.readBigXMLtoLevel(xml, 2)) {
        int lebel = 4;
        qint64 sz = QFileInfo(fileName).size();
        if (sz<(1024000 * 3)) // mb
            lebel = 100;
        setWindowTitle(fileName+" - BigXmlReader"); // даже если файл считан с ошибками, должны знать что за файл....
        if (!bigxmlWidget.readBigXMLtoLevel(xml, lebel)) {
            QString mess = QString("Parse error in file %1: %2").arg(fileName).arg(bigxmlWidget.errorXMLString(xml));
            mess.replace('\n',' ');

             QListWidgetItem *item = new QListWidgetItem(mess,m_mesages);
             item->setFlags(item->flags() | Qt::ItemIsEditable);
            //toggleViewMessagesAct->setChecked(true);
            m_mesageDock->show();
            QMessageBox::warning(this, tr("BigXmlReader"),
                                 tr("Parse error in file %1:\n\n%2")
                                 .arg(fileName)
                                 .arg(bigxmlWidget.errorXMLString(xml)));
        } else {
            statusBar()->showMessage(tr("File loaded"), 2000);
        }
    }
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }

}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    QSettings set("trdm","bigxmlreader");
    set.setValue("width",width());
    set.setValue("height",height());
}

