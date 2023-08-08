#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "FormMain.h"

class Protocol : public QTableView {
    Q_OBJECT

    FormMain* p;
    QPrinter* printer;
    //virtual void showEvent(QShowEvent*);

public:
    Protocol(FormMain* parent = nullptr);

private slots:
    void placeChanged(QStandardItem*);

public slots:
    void updateProtocol(void);

};

#endif // PROTOCOL_H
