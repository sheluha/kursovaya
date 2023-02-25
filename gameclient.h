#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QtWidgets>
#include <QUdpSocket>

class GameClient: public QWidget
{
Q_OBJECT
private:
    QUdpSocket   *pSocket;
    bool         turn;
    QPushButton  *pcmds[9];
    QPushButton  *createButton();
    void         chekWin();
    void         win();
    int          clientPort;
    QString      buttonString = "O";

public:
    GameClient(QWidget *pwgt = 0, int port = 2423, int clientPort = 2424);
    void setButtonString(QString s);
public slots:
    void ButtonClicked();
    void slotReadDatagram();
    void slotSendDatagram();
};
#endif // GAMECLIENT_H



