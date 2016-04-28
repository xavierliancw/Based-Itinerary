/**
  @file
  @author Xavier
  @date 28 April 2016
  @brief This file contains the editdistances class methods.
  */
#include "editdistances.h"
#include "ui_editdistances.h"

#include <QDebug>

editdistances::editdistances(QWidget *parent):QDialog(parent),
ui(new Ui::editdistances){ui->setupUi(this);}

editdistances::editdistances(QWidget *parent, Data inData) :
    QDialog(parent),
    ui(new Ui::editdistances)
{
    ui->setupUi(this);
    data = inData;
    editOk = true;

    //GUI initializations
    this->setWindowFlags(this->windowFlags()
                         & ~Qt::WindowContextHelpButtonHint);
    ui->savedLbl->hide();
    ui->tblSavedLbl->hide();
    ui->tabWidget->setCurrentIndex(0);

    //Populate both lists with stadiums
    for (unsigned int x = 0; x < data.size(); x++)
    {
        ui->firStadLst->addItem(data.getStadName(x));
        ui->secStadLst->addItem(data.getStadName(x));
    }
    ui->firStadLst->setCurrentRow(0);
    ui->secStadLst->setCurrentRow(0);

    refreshMatrix();
}

editdistances::~editdistances()
{delete ui;}

void editdistances::on_modBt_clicked()
{
    QString input = ui->distLE->text();     //Input from line edit
    QIntValidator validate(-1,24901,NULL);  //Input validator
    int valid = 0;                          //Validator condition

    //Validate the text in the line edit
    valid = validate.validate(input,valid);
    if (valid == 0)
    {
        QMessageBox::warning(this, tr("Invalid Input"),
                             tr("Only values from -1 to 24,901 are "
                                "acceptable."),
                             QMessageBox::Ok);
    }
    //If there's no input, don't do anything
    else if (ui->distLE->text() == "")
    {}
    //If both selected stadiums are the same, don't allow it
    else if (ui->firStadLst->currentRow() == ui->secStadLst->currentRow())
    {
        QMessageBox::warning(this, tr("Same Stadium"),
                             tr("Cannot edit stadium's distance to "
                                "itself."),
                             QMessageBox::Ok);
    }
    //If a stadium isn't selected, throw an error
    else if (ui->firStadLst->currentRow() == -1
             || ui->secStadLst->currentRow() == -1)
    {
        QMessageBox::warning(this, tr("Not Enough Selections"),
                             tr("Select a stadium from each list."),
                             QMessageBox::Ok);
    }
    //Otherwise save the new distance
    else
    {
        //Change the data
        data.addDist(ui->firStadLst->currentRow(),
                     ui->secStadLst->currentRow(),
                     ui->distLE->text().toInt());

        //Make save label appear
        ui->savedLbl->show();

        //Make the save label fade out
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->savedLbl->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(10000);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        connect(a,SIGNAL(finished()),this,SLOT(hide_savedLbl()));

        //Refresh the matrix
        refreshMatrix();
    }
}

void editdistances::hide_savedLbl()
{ui->savedLbl->hide();ui->tblSavedLbl->hide();}

void editdistances::on_distLE_returnPressed()
{on_modBt_clicked();}

void editdistances::on_saveBt_clicked()
{
    emit throwUpdatedData(data);
    this->close();
}

void editdistances::on_closeBt_clicked()
{this->close();}

void editdistances::on_firStadLst_currentRowChanged()
{refreshDist();}

void editdistances::on_secStadLst_currentRowChanged()
{refreshDist();}

void editdistances::refreshDist()
{
    int here = ui->firStadLst->currentRow();
    int there = ui->secStadLst->currentRow();

    //Only grab distance if both lists are greater than -1
    if (here >= 0 && there >= 0)
    {
        ui->distLE->setText(QString::number
                            (data.getDistBetween(here,there)));
    }
}

void editdistances::refreshMatrix()
//Redraw the matrix
{
    editOk = false;
    ui->matrixTbl->setColumnCount(data.size());
    ui->matrixTbl->setRowCount(data.size());
    for (unsigned int x = 0; x < data.size(); x++)
    {
        ui->matrixTbl->setHorizontalHeaderItem(x,new QTableWidgetItem
                                               (QString::number(x)));
        for (unsigned int y = 0; y < data.size(); y++)
        {
            ui->matrixTbl->setVerticalHeaderItem(y,new QTableWidgetItem
                                                 (QString::number(y)));
            ui->matrixTbl
              ->setItem(x,y,new QTableWidgetItem
                        (QString::number(data.getDistBetween(x,y))));
            if (x == y)
            {
                ui->matrixTbl
                  ->item(x,y)->setBackgroundColor(Qt::black);
            }
        }
    }
    ui->matrixTbl->resizeColumnsToContents();
    editOk = true;
}

void editdistances::on_matrixTbl_cellClicked(int row, int column)
//Display stadium names when a cell is clicked
{
    if (row >= 0)
    {
        ui->firStadLbl->setText(data.getStadName(row) + ",");
    }
    if (column >= 0)
    {
        ui->secStadLbl->setText(data.getStadName(column));
    }
}

void editdistances::on_matrixTbl_cellChanged(int row, int column)
//Allow editing through double clicking a cell
{if (editOk) {
    QString input;                          //Input from table
    QIntValidator validate(-1,24901,NULL);  //Input validator
    int valid = 0;                          //Validator condition

    input = ui->matrixTbl->item(row,column)->text();

    //Validate the text in the line edit
    valid = validate.validate(input,valid);
    if (valid == 0)
    {
        QMessageBox::warning(this, tr("Invalid Input"),
                             tr("Only values from -1 to 24,901 are "
                                "acceptable."),
                             QMessageBox::Ok);
    }
    //Don't allow editing when both selections are the same stadium
    else if (row == column)
    {
        QMessageBox::warning(this, tr("Same Stadium"),
                             tr("Cannot edit stadium's distance to "
                                "itself."),
                             QMessageBox::Ok);
    }
    else
    {
        //Change the data
        data.addDist(row,column,input.toInt());

        //Make save label appear
        ui->tblSavedLbl->show();

        //Make the save label fade out
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->tblSavedLbl->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(10000);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        connect(a,SIGNAL(finished()),this,SLOT(hide_savedLbl()));
    }
    refreshMatrix();
    refreshDist();
}}
