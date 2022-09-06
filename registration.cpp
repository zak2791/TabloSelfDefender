#include "registration.h"
#include <QTimer>
#include <QNetworkDatagram>
#include "settings.h"

Registration::Registration(int p, QWidget* parent) : QWidget(parent){
    par = parent;
    port = p;
    port_klient = "";

    s = new QUdpSocket();
    s->bind(port);

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), SLOT(timeout()));
    connect(s, SIGNAL(readyRead()), SLOT(slotProcessDatagrams()));
    startReg = false;
    timer->start(5000);
    connect(par, SIGNAL(setPort(QString)), this, SLOT(setPort(QString)));
}

void Registration::setPort(QString p){
    timerReg->stop();
    port_klient = p;

    s->writeDatagram(port_klient.toUtf8().data(), 4, address, port); // (port_klient, addr)

    emit registrationSuccesfull(data, "", port_klient); //data[4:].decode(), str(addr[0]), self.port_klient)

    qDebug() << p;
}

Registration::~Registration(void){
    s->close();
}

void Registration::slotProcessDatagrams(){
    QByteArray baDatagram;
    uint val = 0;

    do{
        baDatagram.resize(s->pendingDatagramSize());
        s->readDatagram(baDatagram.data(), baDatagram.size(), &address);

    }while(s->hasPendingDatagrams());
    timer->stop();
    QNetworkDatagram d;
    qDebug() << baDatagram;
    data = QString(baDatagram);
    data.remove(0, 4);
    QString code = QString(baDatagram);
    code.truncate(4);
    if(!startReg){
        startReg = true;
        emit registrationStart(code);

        timerReg = new QTimer();    // таймер времени на нажатие кнопок регистрации
        connect(timerReg, SIGNAL(timeout()), SLOT(timeoutReg()));
        timerReg->start(5000);
    }
}

void Registration::timeout(){
    if(!startReg){
        startReg = true;
        emit registrationFailed();
    }
}

void Registration::timeoutReg(){
    startReg = false;
    emit registrationFailed();
}

