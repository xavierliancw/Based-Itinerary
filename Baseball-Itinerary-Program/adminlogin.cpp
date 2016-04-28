/**
  @file
  @author Xavier
  @date 28 April 2016
  @brief This file contains the methods for the AdminLogin class.
  */
#include "adminlogin.h"
#include "ui_adminlogin.h"

AdminLogin::AdminLogin(QWidget *parent):QDialog(parent),
    ui(new Ui::AdminLogin)
//Default window constructor
{
    ui->setupUi(this);
    password = "1234";
    ui->passLE->setFocus();
    ui->invPassLbl->setText("");
    this->setWindowFlags(this->windowFlags()
                         & ~Qt::WindowContextHelpButtonHint);
}

AdminLogin::~AdminLogin()
//Default window destructor
{delete ui;}

void AdminLogin::on_loginBt_clicked()
//When login button is pressed
{
    //If password is good
    if (ui->passLE->text() == password)
    {
        emit throwLoginStatus(true);
        this->close();
    }
    //If password is not good
    else
    {
        ui->invPassLbl->setText("Invalid password");
        ui->passLE->clear();
        ui->passLE->setFocus();
    }
}

void AdminLogin::on_cancelBt_clicked()
//Closes window
{
    this->close();
}

void AdminLogin::on_passLE_returnPressed()
//Attempt to login when return is pressed
{
    on_loginBt_clicked();
}
