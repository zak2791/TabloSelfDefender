#ifndef UDP_H
#define UDP_H
#include <QWidget>
#include <QUdpSocket>

class Udp : public QWidget {
    Q_OBJECT

private:
    int port;
    QUdpSocket* s_udp;
    int count;
    bool state;
    //QWidget* par;
    bool flagErase = false;

//virtual void timerEvent(QTimerEvent*);

private slots:
    void slotProcessDatagrams();
    void timeout();

public:
    Udp(int port, QWidget *parent = nullptr);
    void sendSignal(int);
    void setTask(int);
    int task;
    QString ID;

public slots:
    void setPort(int);
    void erase(void);

signals:
    void isConnect(int, int);
    void pultData(int, int);
    void sigErase(int);

};

#endif // UDP_H
