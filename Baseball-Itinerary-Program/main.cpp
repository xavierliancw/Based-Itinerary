/**
  @file
  @author jerryberry
  @date 28 April 2016
  @brief This file contains int main() which is the client code.
  */
/*! \mainpage Documentation for the Baseball Itinerary Program
 * \section About
 * This is documentation generated from the source code of the Baseball
 * Itinerary Program.<br>
 * Please use this reference to determine what various blocks of this
 * program does.<br>
 * Click on "Classes" above to view the documentation for
 * the major classes the program uses.<br>
 * Click on "Files" above to browse the program's code.
 *
 * This is a project made for Professor Jerry Lebowitz's Computer Science
 * 1D Spring semester class of 2016 at Saddleback College.
 * @note Functions that do not have complexity documented run in O(1), or
 * constant time.
 * \section Authors
 * Team: jerryberry             <br>
 * Arman Jafarinia              <br>
 * Blake Molina (Product Owner) <br>
 * Ryan Nguyen                  <br>
 * Stephen Lastimosa            <br>
 * Xavier Lian (Scrum Master)
 */
#include "primewin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PrimeWin w(NULL,0);
    w.resize(900,600);

    //Set window icon
    w.setWindowIcon(QIcon(":/defaults/windowicon.png"));
    w.showMaximized();
    w.show();
    return a.exec();
}
