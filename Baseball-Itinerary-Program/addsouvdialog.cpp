/**
  @file
  @author Blake
  @date 28 April 2016
  @brief This file contains the addSouvDialog class methods.
  */
#include "addsouvdialog.h"
#include "ui_addsouvdialog.h"

addSouvDialog::addSouvDialog(Data inData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSouvDialog)
{
    ui->setupUi(this);
    data = inData;
    stadNum = -1;

    // setup table
    ui->souvTableWidget->clear();
    ui->souvTableWidget->setRowCount(0);
    ui->souvTableWidget->setColumnCount(1);
    ui->souvTableWidget->setHorizontalHeaderLabels(QStringList() << "Select Stadium");
    QTableWidgetItem *item; //Item to populate table cell

    //Loop to populate table
    for (unsigned int x = 0; x < data.size(); x++)
    {
        //Add a row
        ui->souvTableWidget->insertRow(ui->souvTableWidget->rowCount());

        //Populate second column with stadName
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(x));
        ui->souvTableWidget->setItem(x,0,item);
    }
    ui->souvTableWidget->resizeColumnsToContents();
}

addSouvDialog::~addSouvDialog()
{
    delete ui;
}


// okay button
void addSouvDialog::on_okBtn_clicked()
{
    // verifies user input all required data
    if(stadNum != -1 && ui->souvNameLE->text() != "")
    {
        QString souvName = ui->souvNameLE->text();
        double souvPrice = ui->souvPriceDB->value();
        data.addSouv(stadNum, souvName, souvPrice);
        // throw data
        emit throwNewSouvData(data);
    }
    else // notify user to enter all info
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Whoops! Missing some info."),
                             QMessageBox::Ok);
    }
}
// cancel button
void addSouvDialog::on_cancelBtn_clicked()
{
    this->close();
}

// allows admin to select which stadium they want to add
// the new souvenir object to
void addSouvDialog::on_souvTableWidget_itemSelectionChanged()
{
    stadNum = ui->souvTableWidget->selectionModel()->currentIndex().row();
}

