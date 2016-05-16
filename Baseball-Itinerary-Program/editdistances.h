/**
  @file
  @author Xavier
  @date 28 April 2016
  @brief This file contains the editdistances class declaration.
  */
#ifndef EDITDISTANCES_H
#define EDITDISTANCES_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QIntValidator>
#include <QMessageBox>

#include "datastructures.h"

namespace Ui {
class editdistances;
}
/**
 * @brief This class handles the dialog window that modifies data
 * pertaining to distances bewteen stadiums and the stadium graph.
 *
 * The dialog window has two listings of the stadiums. When one stadium
 * is seleced from each listing, the distance between them can be
 * modified.<br>
 * The dialog window also has a table that represents the two dimensional
 * matrix representation of the stadium graph. Distances can also be
 * modified in that table.
 * @par REQUIREMENTS:
 * #include <QDialog>                   <br>
 * #include <QGraphicsOpacityEffect>    <br>
 * #include <QPropertyAnimation>        <br>
 * #include <QIntValidator>             <br>
 * #include <QMessageBox>               <br>
 * #include "datastructures.h"
 */
class editdistances : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default constructor
     * @param parent : QWidget parent pointer
     */
    explicit editdistances(QWidget *parent = 0);
    /**
     * @brief Constructs the distance editing editing dialog
     * @param parent : QWidget parent pointer
     * @param inData : Data variable from PrimeWin that gets updated
     */
    editdistances(QWidget *parent, Data inData);

    ~editdistances();
    /**
     * @brief Refresh the views of all stadium listings within the dialog
     * to display changes made.
     */
    void refreshDist();

    /**
     * @brief Refresh the view of the matrix table within the dialog to
     * display changes made.
     */
    void refreshMatrix();

signals:
    /**
     * @brief Throw updated Data object back to PrimeWin
     * @param throwThis : The Data object with changes that gets thrown
     * back to PrimeWin
     * @see PrimeWin::catchDataUpdate()
     */
    void throwUpdatedData(Data throwThis);

private slots:
    void on_modBt_clicked();

    void hide_savedLbl();

    void on_distLE_returnPressed();

    void on_saveBt_clicked();

    void on_closeBt_clicked();

    void on_firStadLst_currentRowChanged();

    void on_secStadLst_currentRowChanged();

    void on_matrixTbl_cellClicked(int row, int column);

    void on_matrixTbl_cellChanged(int row, int column);

private:
    Ui::editdistances *ui;
    Data data;
    bool editOk;
};

#endif // EDITDISTANCES_H
