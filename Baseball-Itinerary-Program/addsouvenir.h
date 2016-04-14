#ifndef ADDSOUVENIR_H
#define ADDSOUVENIR_H

#include <QDialog>

namespace Ui {
class addsouvenir;
}

class addsouvenir : public QDialog
{
    Q_OBJECT

public:
    explicit addsouvenir(QWidget *parent = 0);
    ~addsouvenir();

private:
    Ui::addsouvenir *ui;
};

#endif // ADDSOUVENIR_H
