#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTableWidget>
#include <QPrinter>

class Protocol : public QTableWidget {
    Q_OBJECT

    void printPreviewNeeds(QPrinter*);
    QPrinter* printer;

public:
    Protocol(QWidget* parent = nullptr);
};

#endif // PROTOCOL_H
