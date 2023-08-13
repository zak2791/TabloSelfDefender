#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "FormMain.h"

class Protocol : public QDialog {
    Q_OBJECT

    FormMain* p;
    //QPrinter* printer;
    //virtual void showEvent(QShowEvent*);
    QTableView* tblView;
    QString createHTML(int);

    QString tooltip(int, int);

    QString name_competition;
    QString date;
    //QWidget* p;
    QString st;
    QList<QStringList> name;
    QList<QStringList> rates;
    QList<QStringList> errors;
    QString html;
    void updateProtocol(void);

public:
    Protocol(FormMain* parent = nullptr);

private slots:
    void placeChanged(QStandardItem*);
    void showBrauser(void);
    void printProtocol(void);
    void saveProtocol(void);

};

#endif // PROTOCOL_H
