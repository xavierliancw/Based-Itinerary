#ifndef ADDSOUVDIALOG_H
#define ADDSOUVDIALOG_H

#include <QDialog>
#include "datastructures.h"

namespace Ui {
class addSouvDialog;
}

class addSouvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addSouvDialog(Data inData, QWidget *parent = 0);
    ~addSouvDialog();


signals:
    void throwNewSouvData(Data throwThis);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_souvTableWidget_itemSelectionChanged();

 private:
    int stadNum;
    Data data;
    Ui::addSouvDialog *ui;
};

#endif // ADDSOUVDIALOG_H
