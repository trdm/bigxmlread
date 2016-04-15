/********************************************************************************
** Form generated from reading UI file 'xqueryform.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XQUERYFORM_H
#define UI_XQUERYFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XQueryForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pB_Run;
    QTextEdit *tE_XQText;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPlainTextEdit *pTE_XQTextRes;

    void setupUi(QWidget *XQueryForm)
    {
        if (XQueryForm->objectName().isEmpty())
            XQueryForm->setObjectName(QString::fromUtf8("XQueryForm"));
        XQueryForm->resize(416, 303);
        verticalLayout_3 = new QVBoxLayout(XQueryForm);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        splitter = new QSplitter(XQueryForm);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        pB_Run = new QPushButton(widget);
        pB_Run->setObjectName(QString::fromUtf8("pB_Run"));

        horizontalLayout->addWidget(pB_Run);


        verticalLayout->addLayout(horizontalLayout);

        tE_XQText = new QTextEdit(widget);
        tE_XQText->setObjectName(QString::fromUtf8("tE_XQText"));

        verticalLayout->addWidget(tE_XQText);

        splitter->addWidget(widget);
        widget1 = new QWidget(splitter);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        pTE_XQTextRes = new QPlainTextEdit(widget1);
        pTE_XQTextRes->setObjectName(QString::fromUtf8("pTE_XQTextRes"));

        verticalLayout_2->addWidget(pTE_XQTextRes);

        splitter->addWidget(widget1);

        verticalLayout_3->addWidget(splitter);


        retranslateUi(XQueryForm);

        QMetaObject::connectSlotsByName(XQueryForm);
    } // setupUi

    void retranslateUi(QWidget *XQueryForm)
    {
        XQueryForm->setWindowTitle(QApplication::translate("XQueryForm", "XQuery form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("XQueryForm", "XQuery text", 0, QApplication::UnicodeUTF8));
        pB_Run->setText(QApplication::translate("XQueryForm", "Run", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("XQueryForm", "XQuery text", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XQueryForm: public Ui_XQueryForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XQUERYFORM_H
