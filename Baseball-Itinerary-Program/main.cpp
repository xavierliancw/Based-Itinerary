/**
  @file
  @author jerryberry
  @date 28 April 2016

  This file contains int main() which is the client code.
  */
/*! \mainpage jerryberry's Baseball Itinerary Program
 * \section Uhh section?
 *
 * This is the introduction? lol?
 *
 * \section Another section?
 * \subsection This is a subsection?
 *
 * blah blah ect...
 */
#include "primewin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PrimeWin w(NULL,0);
    w.resize(900,500);
    w.show();
    return a.exec();
}
