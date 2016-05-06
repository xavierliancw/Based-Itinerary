#include "mstprim.h"
#include "ui_mstprim.h"

MstPrim::MstPrim(Data data, QWidget *parent):QDialog(parent),
ui(new Ui::MstPrim)
{
    //GUI setup
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()
                         & ~Qt::WindowContextHelpButtonHint);

    vector<pair<int,int> > edges;       //Vector of edges
    int mileage = data.askPrim(edges);  //Ask Prim to get mileage
    QTableWidgetItem *item;             //QTable item

    //Display total mileage
    ui->mileageLbl->setText("Minimum Spanning Tree Mileage: "
                            + QString::number(mileage));

    //Fill table with Prim's edges
    ui->edgeTbl->setRowCount(edges.size());
    ui->edgeTbl->setColumnCount(4);
    for (int x = 0; x < (int)edges.size(); x++)
    {
        //First column has from stadium
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(edges.at(x).first));
        item->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
        ui->edgeTbl->setItem(x,0,item);

        //Second column is just a visual thing
        item = new QTableWidgetItem;
        item->setData(0,"-");
        item->setTextAlignment(Qt::AlignCenter);
        ui->edgeTbl->setItem(x,1,item);

        //Third column has to stadium
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(edges.at(x).second));
        ui->edgeTbl->setItem(x,2,item);

        //Fourth column has the distance between
        item = new QTableWidgetItem;
        item->setData(0,QString::number(data.getDistBetween
                                        (edges.at(x).first,
                                         edges.at(x).second)) + " miles");
        ui->edgeTbl->setItem(x,3,item);
    }
    ui->edgeTbl->resizeColumnsToContents();
}

MstPrim::~MstPrim()
{delete ui;}
