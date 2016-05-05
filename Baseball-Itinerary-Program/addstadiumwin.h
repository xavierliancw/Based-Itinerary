/**
  @file
  @author Blake
  @date 02 May 2016
  @brief This file contains the addStadiumWin class declaration.
  */

#ifndef ADDSTADIUMWIN_H
#define ADDSTADIUMWIN_H
#include <QDialog>
#include <QMessageBox>
#include "datastructures.h"


namespace Ui {
class AddStadiumWin;
}
/**
 * @brief This class handles the dialog window that adds new Teams
 * and corresponding Stadiums to the Stadium vector (mastervect).
 *
 * @par REQUIREMENTS:
 * #include <QDialog>           <br>
 * #include "datastructures.h"
 */
class AddStadiumWin : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default constuctor that constructs the souvenir adding
     * dialog
     * @param inData : Data variable from PrimeWin that gets updated
     * @param parent : QWidget parent pointer
     */
    explicit AddStadiumWin(Data inData, QWidget *parent = 0);
    ~AddStadiumWin();

signals:
    /**
     * @brief Throw updated Data object back to PrimeWin
     * @param throwThis : The Data object with changes that gets thrown
     * back to PrimeWin
     * @see PrimeWin::catchDataUpdate()
     */
    void throwNewTeamData(Data throwThis);

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_aLeagueBtn_toggled(bool checked);
    void on_nLeagueButn_toggled(bool checked);


private:
    int stadNum;
    bool aLeague;
    bool nLeague;
    Data data;
    Ui::AddStadiumWin *ui;
};

#endif // ADDSTADIUMWIN_H
