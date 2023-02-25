#include <QApplication>
#include "gameclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameClient client1(0,2423,2424);
    client1.setButtonString("O");
    GameClient client2(0,2424,2423);
    client1.setButtonString("X");

    client1.show();
    client2.show();

    return a.exec();

}
