#ifndef FIRSTROUND_H
#define FIRSTROUND_H

#include <QWidget>
#include <QGridLayout>
#include "fam_reg.h"
#include "Ball.h"

class FirstRound : public QWidget{
    Q_OBJECT

    QLabel* lbl_priem;

    QLabel* lbl_1;
    QLabel* lbl_2;
    QLabel* lbl_3;
    QLabel* lbl_4;
    QLabel* lbl_5;
    QLabel* lbl_sum;
    QLabel* lbl_total;

    void sbros(void);
    void func(void);
    virtual void paintEvent(QPaintEvent*);
    virtual void keyPressEvent(QKeyEvent*);

public:
    FirstRound(QWidget *parent = nullptr);
    QWidget* p;
    QLabel* judge_flag1;
    QLabel* judge_flag2;
    QLabel* judge_flag3;
    QLabel* judge_flag4;
    QLabel* judge_flag5;
    QLabel* lbl_flag;
    QGridLayout* grid;
    Fam* name;
    Fam* region;
    Ball* rate_sum;
    Ball* rate_total;
    Ball* rate_1;
    Ball* rate_2;
    Ball* rate_3;
    Ball* rate_4;
    Ball* rate_5;
    Ball* count_priem;
};

#endif // FIRSTROUND_H
