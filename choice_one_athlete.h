#ifndef CHOICE_ONE_ATHLETE_H
#define CHOICE_ONE_ATHLETE_H
#include <QDialog>
#include "FormMain.h"

class Choice_one_athlete : QDialog{

    Q_OBJECT

private:
    virtual void showEvent(QShowEvent*);

    virtual void closeEvent(QCloseEvent*);
    virtual void resizeEvent(QResizeEvent*);

    QString sep(QString);
    void family_set(void);

    QTableView* tbl;
    QLabel* lbl_fam;
    QLineEdit* inFam;
    QStringList l;
    FormMain* p;                 // родительский widget
    int col;
    QString sel_data;
    QString b;
    QString r;



private slots:
    void sel(QItemSelection, QItemSelection);
    void textEdited(QString);

public:
    Choice_one_athlete(QStringList, FormMain* parent = nullptr);

};

#endif // CHOICE_ONE_ATHLETE_H
