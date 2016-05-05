/**
  @file
  @author Blake
  @date 28 April 2016
  @brief This file contains the addSouvDialog class declaration.
  */
#ifndef ADDSOUVDIALOG_H
#define ADDSOUVDIALOG_H
#include <QDialog>
#include "datastructures.h"
#include <QMessageBox>
#include <QPushButton>

namespace Ui {
class addSouvDialog;
}
/**
 * @brief This class handles the dialog window that adds new souvenirs
 * to stadiums.
 *
 * This dialog has a listing of stadiums. When one is selected, the
 * a new souvenir can be added to that stadium.
 * @par REQUIREMENTS:
 * #include <QDialog>           <br>
 * #include "datastructures.h"
 */
class addSouvDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default constuctor that constructs the souvenir adding
     * dialog
     * @param inData : Data variable from PrimeWin that gets updated
     * @param parent : QWidget parent pointer
     */
    explicit addSouvDialog(Data inData, QWidget *parent = 0);
    ~addSouvDialog();

signals:
    /**
     * @brief Throw updated Data object back to PrimeWin
     * @param throwThis : The Data object with changes that gets thrown
     * back to PrimeWin
     * @see PrimeWin::catchDataUpdate()
     */
    void throwNewSouvData(Data throwThis, int stadChanged);

private slots:
    void on_souvTableWidget_itemSelectionChanged();
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();

private:
    int stadNum;
    Data data;
    Ui::addSouvDialog *ui;
};
#endif // ADDSOUVDIALOG_H
