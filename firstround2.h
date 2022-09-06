#ifndef FIRSTROUND2_H
#define FIRSTROUND2_H

#include <QWidget>
#include <QGridLayout>
#include "fam_reg.h"
#include "Ball.h"

class FirstRound2 : public QWidget{
    Q_OBJECT





    QLabel* lbl_1;
    QLabel* lbl_2;
    QLabel* lbl_3;
    QLabel* lbl_4;
    QLabel* lbl_5;
    QLabel* lbl_sum;
    //QLabel* lbl_total;

    QLabel* s1;
    QLabel* s2;
    QLabel* s3;
    QLabel* s4;
    QLabel* s5;

    QLabel* lbl_empty_top;
    QLabel* lbl_empty_bottom;
    QLabel* space_bottom;








    int space;

    void sbros(void);
    void func(void);
    virtual void paintEvent(QPaintEvent*);
    virtual void keyPressEvent(QKeyEvent*);

public:
    FirstRound2(QWidget *parent = nullptr);
    QWidget* p;
    QLabel* lbl_flag_blue;
    QLabel* lbl_flag_red;
    Fam* region_blue;
    Fam* region_red;
    QGridLayout* grid;
    QLabel* judge_flag1;
    QLabel* judge_flag2;
    QLabel* judge_flag3;
    QLabel* judge_flag4;
    QLabel* judge_flag5;
    Ball* rate_sum;
    Ball* rate_total_blue;
    Ball* rate_total_red;
    Ball* rate_1;
    Ball* rate_2;
    Ball* rate_3;
    Ball* rate_4;
    Ball* rate_5;
    QLabel* lbl_priem;

    Fam* name_blue;

    Fam* name_red;

    Ball* count_priem;
};

#endif // FIRSTROUND2_H
