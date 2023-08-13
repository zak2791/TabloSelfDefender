///////////////////////////////////////////////////////////////////
//                       следующий вопрос                        //
///////////////////////////////////////////////////////////////////
#include "FormMain.h"

void Btn_next_clicked(FormMain* fm){
    bool color;
    fm->Btn_next->setEnabled(false);
    fm->flag_priem += 1;
    fm->GroupBox_2->setEnabled(false);

    for(int count=1;count<6;count++){
        QLineEdit* led = fm->findChild<QLineEdit*>("led" + QString::number(count));
        led->setEnabled(true);
        led->setStyleSheet("QLineEdit {background-color: white;}");
        if(count == 1) led->setFocus();
        led->setText("");
    }
    fm->Lbl_sum->setText("");

    fm->enable_correct = false;     // запрет коррекции результата

    if(fm->flag_mode == 0){
        fm->fr->rate_sum->sbros();
        fm->fr->rate_1->sbros();
        fm->fr->rate_2->sbros();
        fm->fr->rate_3->sbros();
        fm->fr->rate_4->sbros();
        fm->fr->rate_5->sbros();

        fm->Lbl_q->setText("<font color=\"black\"><b>" + QString::number(fm->flag_priem) + "</b></font>");
        fm->fr->count_priem->setText(QString::number(fm->flag_priem));

        fm->pult2->task = fm->flag_priem;
        fm->pult3->task = fm->flag_priem;
        fm->pult4->task = fm->flag_priem;
        fm->pult5->task = fm->flag_priem;
        fm->task = 1;

        fm->Btn_enter->setEnabled(true);
    }else{
        if(fm->flag_priem == 1)
            color = true;
        else if(fm->flag_priem == 2)
            color = false;
        else if(fm->flag_priem == 3)
            color = false;
        else if(fm->flag_priem == 4)
            color = true;
        else if(fm->flag_priem == 5)
            color = true;
        else if(fm->flag_priem == 6)
            color = false;
        else if(fm->flag_priem == 7)
            color = false;
        else if(fm->flag_priem == 8)
            color = true;
        else if(fm->flag_priem == 9)
            color = true;
        else
            color = false;

        if(!fm->RbRed->isChecked())
            color = !color;
        if(color)
            fm->current_color = "red";
        else
            fm->current_color = "blue";
        int task = int((float)(fm->flag_priem) / 2.0 + 0.5);   // текущий вопрос
        fm->fr2->rate_sum->sbros();
        fm->fr2->rate_1->sbros();
        fm->fr2->rate_2->sbros();
        fm->fr2->rate_3->sbros();
        fm->fr2->rate_4->sbros();
        fm->fr2->rate_5->sbros();

        fm->Lbl_q->setText("<font color=" + fm->current_color + "><b>" + QString::number(task) + "</b></font>");

        fm->fr2->lbl_priem->setText("TASK\n" + QString::number(task));
        fm->fr2->lbl_priem->setStyleSheet("QLabel { background-color : black; color : " +  fm->current_color + "; }");

        fm->pult2->task = task;
        fm->pult3->task = task;
        fm->pult4->task = task;
        fm->pult5->task = task;
        fm->task = 1;
        fm->Btn_enter->setEnabled(true);
        fm->Btn_change->setEnabled(false);
    }
}
