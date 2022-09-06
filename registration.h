#ifndef REGISTRATION_H
#define REGISTRATION_H
#include <QWidget>
#include <QUdpSocket>

class Registration : public QWidget {
    Q_OBJECT

private:
    QWidget* par;
    int port;
    QUdpSocket* s;
    QString port_klient;
    bool startReg;  // флаг начала регистрации
    QTimer* timer;
    QTimer* timerReg;
    QString data;
    QHostAddress address;

private slots:
    void slotProcessDatagrams();
    void timeout();
    void timeoutReg();
    void setPort(QString);

public:
    Registration(int port, QWidget *parent = nullptr);
    ~Registration(void);


signals:
    void registrationStart(QString);
    void registrationSuccesfull(QString, QString, QString);
    void registrationFailed();
};

#endif // REGISTRATION_H
