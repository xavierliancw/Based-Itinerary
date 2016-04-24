#include "addsouvdialog.h"
#include "ui_addsouvdialog.h"

addSouvDialog::addSouvDialog(Data inData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSouvDialog)
{
    ui->setupUi(this);
    data = inData;

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
// Adds new Souv name and new Souv price to master vector
void addSouvDialog::on_buttonBox_accepted()
{
    QString souvName = ui->souvNameLE->text();
    double souvPrice = ui->souvPriceDB->value();
    data.addSouv(stadNum, souvName, souvPrice);
    // throw data
    emit throwNewSouvData(data);
}

// cancel button
void addSouvDialog::on_buttonBox_rejected()
{
    this->close();
}

// allows admin to select which stadium they want to add
// the new souvenir object to
void addSouvDialog::on_souvTableWidget_itemSelectionChanged()
{
    stadNum = ui->souvTableWidget->selectionModel()->currentIndex().row();
}
