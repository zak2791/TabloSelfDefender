#ifndef FORMCORRECTION_H
#define FORMCORRECTION_H

#include "ui_FormCorrection.h"
#include <QLabel>

class FormCorrection : public QWidget, private Ui::FormCorrection {
    Q_OBJECT

private:
    QString style;
    QString styleErr;
    bool stateErr1;
    bool stateErr2;
    bool stateErr3;
    bool stateErr4;
    bool stateErr5;
    bool stateErr6;
    bool stateErr7;
    bool stateErr8;
    bool stateErr9;
    bool stateErr10;
    bool stateErr11;
    bool stateErr12;
    bool stateErr13;
    bool stateErr14;
    bool stateErr15;
    bool statePlusOne;
    bool stateZero;

    QWidget* p;
    QLabel* parent_led;
    QWidget* parent_form;

    QLabel* objRate;
    QLabel* objSum;

    QRegExp reg;

    virtual void closeEvent(QCloseEvent*);
    void record_to_baza(void);

private slots:
    void btn1_clicked(void);
    void btn2_clicked(void);
    void btn3_clicked(void);
    void btn4_clicked(void);
    void btn5_clicked(void);
    void btn6_clicked(void);
    void btn7_clicked(void);
    void btn8_clicked(void);
    void btn9_clicked(void);
    void btn10_clicked(void);
    void btn11_clicked(void);
    void btn12_clicked(void);
    void btn13_clicked(void);
    void btn14_clicked(void);
    void btn15_clicked(void);
    void btnPlus_clicked(void);
    void btnZero_clicked(void);
    void btnOk_clicked(void);
    void btnCancel_clicked(void);
public:
    FormCorrection(QLabel*, QWidget*, QWidget *parent = nullptr);

};

#endif // FORMCORRECTION_H
