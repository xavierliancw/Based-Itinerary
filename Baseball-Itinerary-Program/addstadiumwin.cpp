#include "addstadiumwin.h"
#include "ui_addstadiumwin.h"

AddStadiumWin::AddStadiumWin(Data inData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStadiumWin)
{
    ui->setupUi(this);

    stadNum = 0;
    aLeague = false;
    nLeague = false;
    data = inData;

}

AddStadiumWin::~AddStadiumWin()
{
    delete ui;
}

// okay button
void AddStadiumWin::on_okBtn_clicked()
{
    // verifies user input all required data
    if(ui->newNameLE->text() != "" && (aLeague != true || nLeague != true))
    {
        // new team info
        QString newTeamName = ui->newNameLE->text();
        QString leagueType;
        if(aLeague)
        {
            leagueType = "American League";
        }
        else
        {
           leagueType = "National League";
        }
        // new stadium info
        QString stadName     = ui->stadNameLE->text();
        QString stadPhoneNum = ui->stadPhone->text();
        QString stadAddress  = ui->stadAddress->text();
        int stadCapacity     = ui->stadCapacity->value();
        QString turfType     = ui->turfType->text();
        QString openDate     = ui->openDate->text();
        QString stadType     = ui->stadType->text();

        // fill data, then emit data
        // adds new team
        // adds new stadium
        data.addStad(stadName, stadAddress, stadPhoneNum,
                     openDate, stadCapacity, turfType, stadType);
        data.addTeam(data.getStadListSize()+1, newTeamName, leagueType); // ***causing crashes ***
        emit throwNewTeamData(data);
    }
    else // notify user to enter all info
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Whoops! Missing some info."),
                             QMessageBox::Ok);
    }
}

// cancel button
void AddStadiumWin::on_cancelBtn_clicked()
{
   this->close();
}


// American League radio button
void AddStadiumWin::on_aLeagueBtn_toggled(bool checked)
{
    if(checked)
    {
        aLeague = true;
    }
}

// National League radio button
void AddStadiumWin::on_nLeagueButn_toggled(bool checked)
{
    if(checked)
    {
       nLeague = true;
    }
}
