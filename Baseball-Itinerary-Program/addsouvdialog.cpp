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
    this->setWindowFlags(this->windowFlags()
                         & ~Qt::WindowContextHelpButtonHint);

    // setup table
    ui->souvTableWidget->clear();
    ui->souvTableWidget->setRowCount(0);
    ui->souvTableWidget->setColumnCount(1);
    ui->souvTableWidget->setHorizontalHeaderLabels(QStringList()
                                                   << "Select a Stadium");
    QTableWidgetItem *item; //Item to populate table cell

    //Loop to populate table
    for (unsigned int x = 0; x < data.size(); x++)
    {
        //Add a row
        ui->souvTableWidget->insertRow(ui->souvTableWidget->rowCount());

        //Populate second column with stadName
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(x));
        item->setTextAlignment(Qt::AlignCenter);
        ui->souvTableWidget->setItem(x,0,item);
    }
    ui->souvTableWidget->resizeColumnsToContents();
}

addSouvDialog::~addSouvDialog()
{delete ui;}

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
<<<<<<< HEAD
        emit throwNewSouvData(data);
=======
        emit throwNewSouvData(data, stadNum);
>>>>>>> master
        this->close();
    }
    else // notify user to enter all info
    {
        if (ui->souvNameLE->text() == "")
        {
            QMessageBox::warning(this, tr("Whoops! Missing Some Info!"),
                                 tr("Name your souvenir."),
                                 QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(this, tr("Whoops! Missing Some Info!"),
                                 tr("A stadium is not selected."),
                                 QMessageBox::Ok);
        }
    }
}
// cancel button
void addSouvDialog::on_cancelBtn_clicked()
{this->close();}

// allows admin to select which stadium they want to add
// the new souvenir object to
void addSouvDialog::on_souvTableWidget_itemSelectionChanged()
{
    stadNum = ui->souvTableWidget->selectionModel()->currentIndex().row();
    ui->stadFeedbackLbl->setText("Selected Stadium: "
                                 + data.getStadName(stadNum));
}

