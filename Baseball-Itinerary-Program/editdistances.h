#ifndef EDITDISTANCES_H
#define EDITDISTANCES_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QIntValidator>
#include <QMessageBox>

#include "datastructures.h"

namespace Ui {
class editdistances;
}

class editdistances : public QDialog
{
    Q_OBJECT

public:
    explicit editdistances(QWidget *parent = 0);

    editdistances(QWidget *parent, Data inData);

    ~editdistances();

    void refreshDist();

    void refreshMatrix();

signals:
    void throwUpdatedData(Data throwThis);

private slots:
    void on_modBt_clicked();

    void hide_savedLbl();

    void on_distLE_returnPressed();

    void on_saveBt_clicked();

    void on_closeBt_clicked();

    void on_firStadLst_currentRowChanged();

    void on_secStadLst_currentRowChanged();

    void on_matrixTbl_cellClicked(int row, int column);

    void on_matrixTbl_cellChanged(int row, int column);

private:
    Ui::editdistances *ui;
    Data data;
    bool editOk;
};

#endif // EDITDISTANCES_H
