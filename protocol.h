#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "FormMain.h"

class Protocol : public QWidget {
    Q_OBJECT

    FormMain* p;
    QPrinter* printer;
    //virtual void showEvent(QShowEvent*);
    QTableView* tblView;
    QString createHTML(int,
                       QStringList,
                       QList<QStringList>,
                       QList<QStringList>);

    QString tooltip(int, int);

    QString name_competition;
    QString date;
    //QWidget* p;
    QString st;
    QList<QStringList> name;
    QList<QStringList> rates;
    QList<QStringList> errors;

public:
    Protocol(FormMain* parent = nullptr);

private slots:
    void placeChanged(QStandardItem*);

public slots:
    void updateProtocol(void);

};

#endif // PROTOCOL_H
