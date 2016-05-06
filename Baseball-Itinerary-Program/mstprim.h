#ifndef MSTPRIM_H
#define MSTPRIM_H

#include <QDialog>
#include <vector>
#include <utility>

#include "datastructures.h"
using namespace std;

namespace Ui {class MstPrim;}

class MstPrim : public QDialog
{
    Q_OBJECT

public:
    explicit MstPrim(Data data, QWidget *parent = 0);
    ~MstPrim();

private:
    Ui::MstPrim *ui;
};

#endif // MSTPRIM_H
