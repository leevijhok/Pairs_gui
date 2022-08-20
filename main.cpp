//The memory game made using QT UI.
//All UI objects are defined in mm.hh and mm.cpp.
//players.hh and players.cpp has the player-information.
//Can have more than up to 4 players.
//Can have games with up to 24 cards.

//The MainWindow is started here.


// Creator : Leevi Hokkanen,
// Studentnumber : 050253975
// Username : tbleho
// E-mail : leevi.hokkanen@tuni.fi

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
