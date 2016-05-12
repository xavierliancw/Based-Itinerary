#include "addstadiumwin.h"
#include "ui_addstadiumwin.h"

AddStadiumWin::AddStadiumWin(Data inData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStadiumWin)
{
    //Gui initialization
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()
                         & ~Qt::WindowContextHelpButtonHint);
    QIntValidator *validate = new QIntValidator(0,31,this);
    ui->openDate->setValidator(validate);
    QIntValidator *yearVal = new QIntValidator(1839,9999,this);
    ui->yearLE->setValidator(yearVal);

    stadNum = 0;
    aLeague = false;
    nLeague = false;
    data =    inData;
}

AddStadiumWin::~AddStadiumWin()
{
    delete ui;
}

// okay button
void AddStadiumWin::on_okBtn_clicked()
{
    // verifies user input all required data
    if(ui->newNameLE->text() != ""
       && (aLeague != false || nLeague != false)
       && ui->stadNameLE->text() != "" && ui->stadPhone->text() != ""
       && ui->stadAddress->text() != "" && ui->stadCapacity != 0
       && ui->openDate->text() != "" && ui->turfType->text() != ""
       && ui->stadType->text() != ""
       && ui->monthBox->currentIndex() != 0
       && ui->yearLE->text() != "")
    {
        //Validate date
        bool dateOk = true;
        switch (ui->monthBox->currentIndex())
        {
        case 2://Feb
            if (ui->openDate->text().toInt() > 29)
            {dateOk = false;}
            break;
        case 4://Apr
            if (ui->openDate->text().toInt() > 30)
            {dateOk = false;}
            break;
        case 6://Jun
            if (ui->openDate->text().toInt() > 30)
            {dateOk = false;}
            break;
        case 9://Sep
            if (ui->openDate->text().toInt() > 30)
            {dateOk = false;}
            break;
        case 11://Nov
            if (ui->openDate->text().toInt() > 30)
            {dateOk = false;}
            break;
        }
        //If a year is entered before baseball was invented
        if (ui->yearLE->text().toInt() < 1839)
        {dateOk = false;}

        //Stop this function if a date is invalid
        if (!dateOk)
        {
            QMessageBox::warning(this, tr("Date Error"),
                                 tr("Check the date."),
                                 QMessageBox::Ok);
            return;
        }
        //Format date for storage
        QString formattedDate = "";

        if (ui->openDate->text().toInt() < 10)
        {
            formattedDate.append("0");
        }
        formattedDate.append(ui->openDate->text());
        formattedDate.append(" ");
        if (ui->monthBox->currentIndex() < 10)
        {
            formattedDate.append("0");
        }
        formattedDate.append(QString::number(ui->monthBox
                                               ->currentIndex()));
        formattedDate.append(" ");
        formattedDate.append(ui->yearLE->text());

        // new team info
        QString newTeamName = ui->newNameLE->text();
        QString leagueType;
        if(aLeague)
        {
            leagueType = "American";
        }
        else
        {
           leagueType = "National";
        }
        // new stadium info
        QString stadName     = ui->stadNameLE->text();
        QString stadPhoneNum = ui->stadPhone->text();
        QString stadAddress  = ui->stadAddress->text();
        int stadCapacity     = ui->stadCapacity->value();
        QString turfType     = ui->turfType->text();
        QString stadType     = ui->stadType->text();

        // fill data, then emit data
        // adds new team
        // adds new stadium
        data.addStad(stadName, stadAddress, stadPhoneNum, formattedDate,
                     stadCapacity, turfType, stadType, true);
        data.addTeam(data.size() - 1, newTeamName, leagueType);

        emit throwNewTeamData(data);
        emit throwRefreshCmd();
        this->close();
    }
    else // notify user there's missing information
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

QString AddStadiumWin::phoneCheck(QString phone)
//Validates phone numbers and returns a formatted number
{
    QString bareNumber;     //Cleaned up input
    bool hasPlus = false;   //Determines if there's a plus or not
    bool morePlus = false;  //Determines if there are duplicate + signs
    bool throwGen = false;  //Determines if generic error is necessary

    //Iterate through all characters, keeping only + and numbers
    for(QString::iterator it = phone.begin(); it != phone.end(); it++)
    {
        if (*it == '0' || *it == '1' || *it == '2' || *it == '3'
            || *it == '4' || *it == '5' || *it == '6' || *it == '7'
            || *it == '8' || *it == '9' || *it == '+')
        {
            bareNumber += *it;

            //If there's a plus, flag it
            if (*it == '+')
            {
                //If hasPlus is already true, then mark flag duplicate
                if (hasPlus)
                {
                    morePlus = true;
                }
                //Otherwise just mark it true
                else
                {
                    hasPlus = true;
    }   }   }   }
    //Check if there are duplicate +'s
    if (morePlus)
    {
        QMessageBox::warning(this, tr("Invalid Phone Number"),
                             tr("There can only be one \"+\" in a "
                                "phone number."),
                             QMessageBox::Ok);
        return "NULL";
    }
    //Check if phone number is the correct length if it has a +
    if (hasPlus && (bareNumber.size() - bareNumber.indexOf("+") == 12
                    || bareNumber.size() - bareNumber.indexOf("+") == 11))
    {}//It's good, I'm just too lazy to DeMorganize the conditional
    //Check if phone number has the correct length if there's no +
    else if (!hasPlus && (bareNumber.size() == 11
                          || bareNumber.size() == 10))
    {}//Same deal
    else
    {throwGen = true;}

    //Format the number if nothing's wrong
    if (!throwGen)
    {
        //Insert the start of the area code
        //If there's a +
        if (hasPlus)
        {
            //If the size is 11
            if (bareNumber.size() - bareNumber.indexOf("+") == 12)
            {
                //Insert space after the first character after the +
                bareNumber.insert(bareNumber.indexOf("+") + 2, " (");
            }
            //If the size is 10
            else
            {
                //Insert ( after the +
                bareNumber.insert(bareNumber.indexOf("+") + 1, "(");
            }
        }
        //If there's no +
        else
        {
            //If size is 11
            if (bareNumber.size() == 11)
            {
                bareNumber.insert(1, " (");
            }
            //If size is 10
            else
            {
                bareNumber.insert(0, "(");
            }
        }
        //Finish area code
        bareNumber.insert(bareNumber.indexOf("(") + 4, ") ");
        //Add the dash
        bareNumber.insert(bareNumber.lastIndexOf(" ") + 4,"-");
        return bareNumber;
    }
    //Otherwise throw the generic error
    else
    {
        QMessageBox::warning(this, tr("Invalid Phone Number"),
                             tr("Phone number is not valid."),
                             QMessageBox::Ok);
        return "NULL";
    }
}

void AddStadiumWin::on_stadPhone_editingFinished()
//Validates phone number
{
    //Validate the phone number
    if (phoneCheck(ui->stadPhone->text()) != "NULL")
    {
        ui->stadPhone->setText(phoneCheck(ui->stadPhone->text()));
    }
    else
    {
        ui->stadPhone->clear();
    }
}
