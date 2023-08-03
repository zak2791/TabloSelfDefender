#include "udp.h"
#include <QDateTime>
#include <QTimer>
#include <QtEndian>
#include <QNetworkDatagram>

//#include "FormMain.h"

Udp::Udp(int p, QWidget* parent) : QWidget(parent){
    port = p;
    s_udp = new QUdpSocket();
    s_udp->bind(port);
    count = 0;
    state = false;
    QTimer* timer = new QTimer();
    task = 0;

    ID = "0";

    connect(timer, SIGNAL(timeout()), SLOT(timeout()));
    connect(s_udp, SIGNAL(readyRead()), SLOT(slotProcessDatagrams()));
    timer->start(1000);

    //par = parent;

}

void Udp::slotProcessDatagrams(){
    QByteArray baDatagram;
    uint val = 0;
    QHostAddress address;
    //FormMain* pForm = static_cast<FormMain*>(par);

    do{
        baDatagram.resize(s_udp->pendingDatagramSize());
        s_udp->readDatagram(baDatagram.data(), baDatagram.size(), &address);

    }while(s_udp->hasPendingDatagrams());

    QByteArray ba = QByteArray(baDatagram);
    QString DataAsString = QString(ba.remove(0, 3));
    //qDebug() << "dtg" << DataAsString << pForm->Conn2;

    if(ID == DataAsString){

        QNetworkDatagram d;
        d.setDestination(address, port);
        d.setData(QByteArray(1, (char)task));
        int ret = s_udp->writeDatagram(d);
        val = (((uchar)baDatagram[2]) << 16) + (((uchar)baDatagram[1]) << 8) + (uchar)baDatagram[0];

        emit pultData(val, port);
        sendSignal(1);
    }

}

void Udp::sendSignal(int sig){
    if(sig){
        if(!state){
            state = true;
            emit isConnect(1, port);
        }
        count = 0;
    }
    else {
        if(count < 5)
            count++;
        else {
            if(state){
                state = false;
                emit isConnect(0, port);
            }
        }
    }
}

void Udp::setTask(int t){
    task = t;
}

void Udp::timeout(){
    sendSignal(0);
}

void Udp::setPort(int p){
    port = p;
    s_udp->abort();
    s_udp->bind(port);
}
