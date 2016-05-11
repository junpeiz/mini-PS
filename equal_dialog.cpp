#include "equal_dialog.h"
#include "ui_equal_dialog.h"

equal_dialog::equal_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equal_dialog)
{
    ui->setupUi(this);
}

equal_dialog::~equal_dialog()
{
    delete ui;
}

void equal_dialog::on_buttonBox_accepted()
{
    emit sendData(true);
}

void equal_dialog::on_buttonBox_rejected()
{
    emit sendData(false);
}
