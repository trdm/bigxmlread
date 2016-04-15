#ifndef XQUERYFORM_H
#define XQUERYFORM_H

#include <QWidget>

namespace Ui {
class XQueryForm;
}

class XQueryForm : public QWidget
{
    Q_OBJECT

public:
    explicit XQueryForm(QWidget *parent = 0);
    ~XQueryForm();

private:
    Ui::XQueryForm *ui;
};

#endif // XQUERYFORM_H
