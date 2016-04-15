#include "xqueryform.h"
#include "ui_xqueryform.h"

XQueryForm::XQueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XQueryForm)
{
    ui->setupUi(this);
}

XQueryForm::~XQueryForm()
{
    delete ui;
}
