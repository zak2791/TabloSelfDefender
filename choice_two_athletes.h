#ifndef CHOICE_TWO_ATHLETES_H
#define CHOICE_TWO_ATHLETES_H
#include <QDialog>
#include "FormMain.h"

class MyTableView : public QTableView {
    Q_OBJECT
public:
    MyTableView(QWidget* parent=nullptr);

    virtual void mouseReleaseEvent(QMouseEvent* e);
    signals:
    void lr(int);

};

class Choice_two_athletes : QDialog{

    Q_OBJECT

private:
    virtual void showEvent(QShowEvent*);

    virtual void closeEvent(QCloseEvent*);
    virtual void resizeEvent(QResizeEvent*);

    QString sep(QString);
    void family_set(void);

    MyTableView* tbl;
    QLabel* lbl_fam_red;
    QLabel* lbl_fam_blue;
    QLineEdit* inFam;
    QStringList l;
    FormMain* p;                 // родительский widget
    int col;
    QString sel_data;
    QString b;
    QString r;
    QString red;
    QString blue;

private slots:
    void sel(QItemSelection, QItemSelection);
    void sel_(int);
    void textEdited(QString);

public:
    Choice_two_athletes(QStringList, FormMain* parent = nullptr);

};

#endif // CHOICE_TWO_ATHLETES_H
