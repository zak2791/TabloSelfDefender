#ifndef SHOWREPORTS_H
#define SHOWREPORTS_H

#include <QWidget>
#include <QPrintDialog>
#include <QTextEdit>
#include <QPrinter>

class ShowReports : public QWidget {
    Q_OBJECT

QString createHTML(int);
QString tooltip(int, int);
int __num_round;
QString name_competition;
QString date;
QWidget* p;
QString st;
QList<QStringList> name;
QList<QStringList> rates;
QList<QStringList> errors;
QPrintDialog* dlg;
QTextEdit* textEdit;
QPrinter* printer;

private slots:
    void prt(void);

public:
    ShowReports(int _num_round = 5, QWidget* parent = nullptr);

};

#endif // SHOWREPORTS_H
