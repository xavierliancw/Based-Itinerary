#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QDialog>

namespace Ui {
class AdminLogin;
}

class AdminLogin : public QDialog
{
    Q_OBJECT

public:
    explicit AdminLogin(QWidget *parent = 0);
    ~AdminLogin();

signals:
    throwLoginStatus(bool);

private slots:
    void on_loginBt_clicked();

    void on_cancelBt_clicked();

    void on_passLE_returnPressed();

private:
    Ui::AdminLogin *ui;
    QString password;
};

#endif // ADMINLOGIN_H
