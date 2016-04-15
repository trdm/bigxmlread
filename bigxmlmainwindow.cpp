#include <QtGui>

#include "bigxmlmainwindow.h"
#include "bigxmlreader.h"

MainWindow::MainWindow()
{
    QStringList labels;
    labels << tr("Node/Attribute")  << tr("Value/Comment")<< tr("Child count") ;

    bigxmlWidget.header()->setResizeMode(QHeaderView::ResizeToContents);
    bigxmlWidget.header()->setResizeMode(QHeaderView::Interactive);
    bigxmlWidget.setHeaderLabels(labels);
    setCentralWidget(&bigxmlWidget);
    bigxmlWidget.setColumnWidth(0,250);
    bigxmlWidget.setColumnWidth(1,400);
    bigxmlWidget.setUniformRowHeights(true);

    createActions();
    createMenus();

    statusBar()->showMessage(tr("Ready"));
    connect(&bigxmlWidget,SIGNAL(changeCurPath(QString&)), this,SLOT(changeCurPath(QString&)));
    //void changeCurPath(QString& txt);
    setWindowTitle(tr("BigXmlReader"));
    resize(680, 620);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
        return;
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
                      tr("The <b>BigXmlReader</b> example demonstrates how to read big xml file"));
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

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openFile(QString &fileName)
{
    QXmlStreamReader xml;
    if( bigxmlWidget.openFile(fileName, xml)){
        //if (!bigxmlWidget.readBigXML(xml)) {
        // Было if (!bigxmlWidget.readBigXMLtoLevel(xml, 2)) {
        if (!bigxmlWidget.readBigXMLtoLevel(xml, 4)) {
            QMessageBox::warning(this, tr("BigXmlReader"),
                                 tr("Parse error in file %1:\n\n%2")
                                 .arg(fileName)
                                 .arg(bigxmlWidget.errorXMLString(xml)));
        } else {
            setWindowTitle(fileName);
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

