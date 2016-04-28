/**
  @file
  @author Xavier
  @date 28 April 2016
  @brief This file contains the AdminLogin class which handles the dialog
  window used to log into the administrator functions within the program.
  */
#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QDialog>

namespace Ui {
class AdminLogin;
}
/**
 * @brief This class handles the dialog window that grants access to the
 * administrator functions of the program.
 *
 * The dialog window requests a password, and if the password is correct,
 * PrimeWin switches over to the administrator's screen.
 * @par REQUIREMENTS:
 * #include <QDialog>
 * @see PrimeWin
 */
class AdminLogin : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the login dialog
     * @param parent : QWidget parent pointer
     */
    explicit AdminLogin(QWidget *parent = 0);
    ~AdminLogin();

signals:
    /**
     * @brief Throw a boolean back to PrimeWin
     * @see PrimeWin::catchLoginStatus()
     */
    void throwLoginStatus(bool);

private slots:
    void on_loginBt_clicked();

    void on_cancelBt_clicked();

    void on_passLE_returnPressed();

private:
    Ui::AdminLogin *ui;
    QString password;
};

#endif // ADMINLOGIN_H
