#include <QtGui>
#include <QSettings>
#include <QLineEdit>
#include "bigxmlmainwindow.h"
#include "bigxmlreader.h"

MainWindow::MainWindow()
{    
    QStringList labels;    
    labels << tr("Node/Attribute")  << tr("Value/Comment"); //<< tr("Child count") ;
    m_mesages = 0;
    m_mesageDock = 0;
    m_1MbToBute = 1024000;
    m_maxSizeFileFullOpen = (m_1MbToBute * 90); // 90 Mb
    m_openFileSize = 0;

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
    m_curentPath = set.value("curentPath",QDir::currentPath()).toString();
    int ww = set.value("width",620).toInt();
    int wh = set.value("height",680).toInt();
    ww = qMax(ww, 620);
    wh = qMax(wh, 680);
    resize(ww,wh);

    bigxmlWidget.strFindString = set.value("FindString").toString();
    QStringList itemsSerch = set.value("FindStringList").toStringList();
    QString str;
    int countrItems = 0;
    foreach (str, itemsSerch) {
        m_cBoxSearching->addItem(str);
        countrItems++;
        if (countrItems > 50)
            break;
    }
}

void MainWindow::open()
{
    if (m_curentPath.isEmpty())
        m_curentPath = QDir::currentPath();
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                         m_curentPath,
                                         tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
        return;
    QFileInfo fInfo(fileName);
    m_curentPath = fInfo.dir().path();
    m_curFileName = fileName;
    openFile(fileName);

}

void MainWindow::find()
{
    QLabel* label = new QLabel(tr("Find:"));
    QLineEdit* lineEdit = new QLineEdit;
    updateFindString();
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
        if (m_cBoxSearching->currentText() != bigxmlWidget.strFindString) {
            m_cBoxSearching->insertItem(0,bigxmlWidget.strFindString);
            QLineEdit *le = m_cBoxSearching->lineEdit();
            le->setText(bigxmlWidget.strFindString);
        }
        QString strFilename = bigxmlWidget.getFilename();
        if( bigxmlWidget.openFile(strFilename, xml, false)){
            QString findString = lineEdit->text();
            if( bigxmlWidget.findDataBigXML( xml, findString, 0 )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
            else QMessageBox::warning(this, tr("BigXmlReader"), tr("Can't find %1").arg(findString));
        }
        findDialog.close();
    }
}

void MainWindow::findBy()
{
    QString findStr = m_cBoxCurPath->currentText();
    if (!findStr.isEmpty()){
        QXmlStreamReader xml;
        updateFindString();
        QTreeWidgetItem* item = bigxmlWidget.currentItem();
        if( bigxmlWidget.openFile(bigxmlWidget.strFindString, xml, false)){
            QString findString = bigxmlWidget.strFindString;
            if( bigxmlWidget.findDataBigXML( xml, findString, item )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
            else QMessageBox::warning(this, tr("BigXmlReader"), tr("Can't find %1").arg(findString));
        }
    }
}

void MainWindow::findNext()
{
    QXmlStreamReader xml;
    QTreeWidgetItem* item = bigxmlWidget.currentItem();
    updateFindString();
    if( bigxmlWidget.openFile(bigxmlWidget.strFindString, xml, false)){
        QString findString = bigxmlWidget.strFindString;
        if( bigxmlWidget.findDataBigXML( xml, findString, item )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
        else {
            if( bigxmlWidget.findDataBigXML( xml, findString, 0 )) bigxmlWidget.buildTreeBigXMLToMaxIndex();
            else {
                QString mess = tr("Can't find '%1'").arg(findString);
                showNewMessage(mess);
                //QMessageBox::warning(this, tr("BigXmlReader"), tr("Can't find %1").arg(findString));
            }
        }
    }
}

void MainWindow::propertyCurFile()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr(" BigXmlReader - open curent file name "),
                                             tr("Curent file name:"), QLineEdit::Normal,
                                             m_curFileName, &ok);

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
                         "E-mail: sikuda@yandex.ru "
                         "<br>"
                         "site: <a href = \"https://github.com/sikuda/bigxmlread\">https://github.com/sikuda/bigxmlread</a>"
                         "<br>"
                         "<br>"
                         "Edition of trdm E-mail: trdmval@gmail.com"
                         "<br>"
                         "<a href = \"https://github.com/trdm/bigxmlread\">https://github.com/trdm/bigxmlread</a>"
                         "<br>"
                         "Compiled at %1 %2"
                         "<br>"
                         "Executed path: %3"
                         )).arg(__DATE__).arg(__TIME__).arg(qApp->applicationFilePath()));
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
    // Cur path
    QLabel* label = new QLabel(this);
    label->setText("Cur path:");
    tb->addWidget(label);

    m_cBoxCurPath = new QComboBox(this);
    tb->addWidget(m_cBoxCurPath);
    m_cBoxCurPath->setEditable(true);
    m_cBoxCurPath->setMaximumWidth(200);
    m_cBoxCurPath->setMinimumWidth(200);

    // Searching
    QLabel* label2 = new QLabel(this);
    label2->setText("Searching:");
    tb->addWidget(label2);

    m_cBoxSearching = new QComboBox(this);
    tb->addWidget(m_cBoxSearching);
    m_cBoxSearching->setEditable(true);
    m_cBoxSearching->setMaximumWidth(200);
    m_cBoxSearching->setMinimumWidth(200);
    tb->addAction(findActNext);

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
    toggleViewMessagesAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL+ Qt::Key_Z));
    viewMenu->addAction(toggleViewMessagesAct);
    toggleViewMessagesAct->setChecked(false);
    m_mesageDock->hide();

}

void MainWindow::updateFindString()
{
    QString findStr = m_cBoxSearching->currentText();
    if (!findStr.isEmpty()){
        if (bigxmlWidget.strFindString != findStr)
            bigxmlWidget.strFindString = findStr;
    }
}

void MainWindow::showNewMessage(QString messageStr)
{
    if (messageStr.isEmpty())
        return;
    QString strMes = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    strMes.append(": ").append(messageStr);
    QListWidgetItem *item = new QListWidgetItem(strMes,m_mesages);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    //toggleViewMessagesAct->setChecked(true);
    m_mesageDock->show();
}

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openFile(QString &fileName)
{
    QXmlStreamReader xml;
    m_curFileName = fileName;    
    bool isOpen = bigxmlWidget.openFile(fileName, xml);
    if(isOpen){
        //if (!bigxmlWidget.readBigXML(xml)) {
        // Было if (!bigxmlWidget.readBigXMLtoLevel(xml, 2)) {
        int lebel = 4;
        m_openFileSize = QFileInfo(fileName).size();
        if (m_openFileSize<m_maxSizeFileFullOpen) // mb
            lebel = 100;

        QFileInfo fInfo(fileName);
        m_curentPath = fInfo.dir().path();
        m_curFileName = fileName;

        setWindowTitle(fileName+" - BigXmlReader"); // даже если файл считан с ошибками, должны знать что за файл....
        QString mess;
        if (!bigxmlWidget.readBigXMLtoLevel(xml, lebel)) {
            mess = QString("Parse error in file %1: %2").arg(fileName).arg(bigxmlWidget.errorXMLString(xml));
            mess.replace('\n',' ');
            mess.replace("/","\\");
            showNewMessage(mess);
            QMessageBox::warning(this, tr("BigXmlReader"),
                                 tr("Parse error in file %1:\n\n%2")
                                 .arg(fileName)
                                 .arg(bigxmlWidget.errorXMLString(xml)));
        } else {
            mess = QString("Open file %1").arg(fileName);
            mess.replace('\n',' ');
            statusBar()->showMessage(tr("File loaded"), 2000);
            mess.replace("/","\\");
            showNewMessage(mess);
        }
    }
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    if (isOpen)
        files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
    if (m_openFileSize < (m_1MbToBute*5)) {
        bigxmlWidget.expandAll();
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
    set.setValue("curentPath",m_curentPath);
    set.setValue("FindString",bigxmlWidget.strFindString);
    if (m_cBoxSearching->count() > 0) {
        QStringList items;
        for (int q = 0; q<m_cBoxSearching->count(); q++) {
            items << m_cBoxSearching->itemText(q);
        }
        set.setValue("FindStringList",items);
    }
    ev->accept();
}

