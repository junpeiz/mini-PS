#ifndef EQUAL_DIALOG_H
#define EQUAL_DIALOG_H

#include <QDialog>

namespace Ui {
class equal_dialog;
}

class equal_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit equal_dialog(QWidget *parent = 0);
    ~equal_dialog();

private:
    Ui::equal_dialog *ui;

signals:
    void sendData(bool);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // EQUAL_DIALOG_H
