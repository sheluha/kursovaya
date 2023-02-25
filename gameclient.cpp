#include "gameclient.h"
#include <QtWidgets>

GameClient::GameClient(QWidget *pwgt, int port, int clientPort) : QWidget(pwgt), clientPort(clientPort)
{
    setFixedSize(274,274);
    setWindowTitle("Client");
    pSocket = new QUdpSocket(this);
    pSocket->bind(port);

    connect(pSocket, SIGNAL(readyRead()),
                this, SLOT(slotReadDatagram()));


    int k = 0;
    QGridLayout *pGLayout = new QGridLayout;
    for(int i = 0; i<3; ++i) {
        for(int j = 0; j<3; ++j) {
           QPushButton *but = createButton();
           pcmds[k] = but;
           k++;
           pGLayout->addWidget(but,i,j);
        }
    }
    setLayout(pGLayout);
}

QPushButton* GameClient::createButton()
{
    QPushButton *pbt = new QPushButton("");
    pbt->setMinimumSize(80,80);
    QFont ft = pbt->font();
    ft.setPointSize(23);
    pbt->setFont(ft);
    connect(pbt,SIGNAL(clicked()),SLOT(ButtonClicked()));
    pbt->setStyleSheet(
         "background-color: gray;"
         "border: 1px solid black;"
         "border-radius: 10px;"
         "color: lightGray; "
         "font-size: 25px;"
         );
    return pbt;
}

void GameClient::win()
{
    for (int i =0; i<9; ++i) {
          pcmds[i]->setText("");
          pcmds[i]->blockSignals(false);
    }
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    QPointF p = QPointF(1,1);
    out << p;

    pSocket->writeDatagram(baDatagram,QHostAddress::LocalHost,clientPort);

}

void GameClient::ButtonClicked()
{
    QPushButton *pcmd =  (QPushButton *) sender();
    if (turn) {
       pcmd->setText(buttonString);
       pcmd->blockSignals(true);
       emit slotSendDatagram();
       chekWin();
    }
}

void GameClient::chekWin(){

    if ((pcmds[0]->text() != "")&&(pcmds[0]->text() == pcmds[4]->text())&&(pcmds[0]->text() == pcmds[8]->text())){
         win();
    }
    else if ((pcmds[2]->text() != "")&&(pcmds[2]->text() == pcmds[4]->text())&&(pcmds[2]->text() == pcmds[6]->text())){
         win();
    }
    for (int i = 0; i<3; ++i) {
       if(pcmds[i]->text() != ""){
           if ((pcmds[i]->text() == pcmds[i+3]->text())&&( pcmds[i+3]->text()== pcmds[i+6]->text())){
               win();
           }
       }
    }
    if ((pcmds[0]->text() != "")&&(pcmds[0]->text() == pcmds[1]->text())&&(pcmds[0]->text() == pcmds[2]->text())){
         win();
    }
    else if ((pcmds[3]->text() != "")&&(pcmds[3]->text() == pcmds[4]->text())&&(pcmds[3]->text() == pcmds[5]->text())){
         win();
    }
    else if ((pcmds[6]->text() != "")&&(pcmds[6]->text() == pcmds[7]->text())&&(pcmds[7]->text() == pcmds[8]->text())){
         win();
    }

    bool drawCheck = false;
    for(int i = 0; i<9; ++i){
        if(pcmds[i]->text() == ""){
            drawCheck = false;
            break;
        }
        else{
            drawCheck = true;
        }
    }
    if(drawCheck){
        win();
    }
}

void GameClient::slotSendDatagram()
{
    QPushButton *button = (QPushButton*) sender();
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    QPointF p = button->pos();
    out << p;

    qDebug()<<baDatagram;



    pSocket->writeDatagram(baDatagram,QHostAddress::LocalHost,clientPort);
    turn = false;
}

void GameClient::setButtonString(QString s){
    if (s == "O" || s == "X"){
        buttonString = s;
    }
}

void GameClient::slotReadDatagram()
{
    QByteArray baDatagram;
    do {
        baDatagram.resize(pSocket->pendingDatagramSize());
        pSocket->readDatagram(baDatagram.data(), baDatagram.size());
    } while (pSocket->hasPendingDatagrams());

    QDataStream in(&baDatagram, QIODevice::ReadOnly);

    QPointF p;
    in >> p;
    if (p == QPointF(1,1)){
        win();
    }
    for (int i =0; i<9; ++i) {
       if (pcmds[i]->pos() == p){
           if(buttonString == "O"){
               pcmds[i]->setText("X");
           }
           else{
               pcmds[i]->setText("O");
           }
       }
    }
    turn = true;
}
