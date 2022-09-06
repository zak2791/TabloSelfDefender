#include "formcorrection.h"
#include <QLabel>
#include <QLineEdit>
#include "Ball.h"
#include "FormMain.h"
#include "errors.h"
#include <QRegExp>
#include <QtSql>

FormCorrection::FormCorrection(QLabel* _parent_led, QWidget* _parent_form, QWidget* parent) : QWidget(){
    setupUi(this);
    //---состояние кнопок---#
    style = btn1->styleSheet();
    //styleErr = None
    reg = QRegExp("\\d\\d?[.]?\\d?");

    stateErr1 = false;
    stateErr2 = false;
    stateErr3 = false;
    stateErr4 = false;
    stateErr5 = false;
    stateErr6 = false;
    stateErr7 = false;
    stateErr8 = false;
    stateErr9 = false;
    stateErr10 = false;
    stateErr11 = false;
    stateErr12 = false;
    stateErr13 = false;
    stateErr14 = false;
    stateErr15 = false;
    statePlusOne = false;
    stateZero = false;

    p = parent;

    parent_led = _parent_led;
    parent_form = _parent_form;

    QString s = "lbRate" + parent_led->objectName().remove(0, parent_led->objectName().length() - 2);
    objRate = parent_form->findChild<QLabel*>(s);
    objSum = parent_form->findChild<QLabel*>("lblSum" + parent_led->objectName()[parent_led->objectName().length() - 2]);

    styleErr = parent_led->styleSheet();

    QString text = parent_led->text().replace(" ", "");
    QList<QString> t;

    t = text.split(",");

    foreach(QString each, t){
        if(each == "1"){
            btn1->setStyleSheet("color:#ff0000;");
            stateErr1 = true;
        }
        else if(each == "2"){
            btn2->setStyleSheet("color:#ff0000;");
            stateErr2 = true;
        }
        else if(each == "3"){
            btn3->setStyleSheet("color:#ff0000;");
            stateErr3 = true;
        }
        else if(each == "4"){
            btn4->setStyleSheet("color:#ff0000;");
            stateErr4 = true;
        }
        else if(each == "5"){
            btn5->setStyleSheet("color:#ff0000;");
            stateErr5 = true;
        }
        else if(each == "6"){
            btn6->setStyleSheet("color:#ff0000;");
            stateErr6 = true;
        }
        else if(each == "7"){
            btn7->setStyleSheet("color:#ff0000;");
            stateErr7 = true;
        }
        else if(each == "8"){
            btn8->setStyleSheet("color:#ff0000;");
            stateErr8 = true;
        }
        else if(each == "9"){
            btn9->setStyleSheet("color:#ff0000;");
            stateErr9 = true;
        }
        else if(each == "10"){
            btn10->setStyleSheet("color:#ff0000;");
            stateErr10 = true;
        }
        else if(each == "11"){
            btn11->setStyleSheet("color:#ff0000;");
            stateErr11 = true;
        }
        else if(each == "12"){
            btn12->setStyleSheet("color:#ff0000;");
            stateErr12 = true;
        }
        else if(each == "13"){
            btn13->setStyleSheet("color:#ff0000;");
            stateErr13 = true;
        }
        else if(each == "14"){
            btn14->setStyleSheet("color:#ff0000;");
            stateErr14 = true;
        }
        else if(each == "15"){
            btn15->setStyleSheet("color:#ff0000;");
            stateErr15 = true;
        }
        else if(each == "0"){
            btnZero->setStyleSheet("color:#ff0000;");
            stateZero = true;
        }
        else if(each == "+1"){
            btnPlus->setStyleSheet("color:#ff0000;");
            statePlusOne = true;
        }
    }
    connect(btnZero,    SIGNAL(clicked()), SLOT(btnZero_clicked()));
    connect(btn1,       SIGNAL(clicked()), SLOT(btn1_clicked()));
    connect(btn2,       SIGNAL(clicked()), SLOT(btn2_clicked()));
    connect(btn3,       SIGNAL(clicked()), SLOT(btn3_clicked()));
    connect(btn4,       SIGNAL(clicked()), SLOT(btn4_clicked()));
    connect(btn5,       SIGNAL(clicked()), SLOT(btn5_clicked()));
    connect(btn6,       SIGNAL(clicked()), SLOT(btn6_clicked()));
    connect(btn7,       SIGNAL(clicked()), SLOT(btn7_clicked()));
    connect(btn8,       SIGNAL(clicked()), SLOT(btn8_clicked()));
    connect(btn9,       SIGNAL(clicked()), SLOT(btn9_clicked()));
    connect(btn10,      SIGNAL(clicked()), SLOT(btn10_clicked()));
    connect(btn11,      SIGNAL(clicked()), SLOT(btn11_clicked()));
    connect(btn12,      SIGNAL(clicked()), SLOT(btn12_clicked()));
    connect(btn13,      SIGNAL(clicked()), SLOT(btn13_clicked()));
    connect(btn14,      SIGNAL(clicked()), SLOT(btn14_clicked()));
    connect(btn15,      SIGNAL(clicked()), SLOT(btn15_clicked()));
    connect(btnPlus,    SIGNAL(clicked()), SLOT(btnPlus_clicked()));
    connect(btnOk,      SIGNAL(clicked()), SLOT(btnOk_clicked()));
    connect(btnCancel,  SIGNAL(clicked()), SLOT(btnCancel_clicked()));

}

void FormCorrection::closeEvent(QCloseEvent* e){
    parent_led->setStyleSheet("background-color:#ffffff;");
}

void FormCorrection::btn1_clicked(void){
    if(!stateErr1){
        stateErr1 = true;
        btn1->setStyleSheet("color: red");
    }else{
        stateErr1 = false;
        btn1->setStyleSheet(style);
    }
}

void FormCorrection::btn2_clicked(void){
    if(!stateErr2){
        stateErr2 = true;
        btn2->setStyleSheet("color: red");
    }else{
        stateErr2 = false;
        btn2->setStyleSheet(style);
        }
}

void FormCorrection::btn3_clicked(void){
    if(!stateErr3){
        stateErr3 = true;
        btn3->setStyleSheet("color: red");
    }else{
        stateErr3 = false;
        btn3->setStyleSheet(style);
    }
}

void FormCorrection::btn4_clicked(void){
    if(!stateErr4){
        stateErr4 = true;
        btn4->setStyleSheet("color: red");
    }else{
        stateErr4 = false;
        btn4->setStyleSheet(style);
    }
}

void FormCorrection::btn5_clicked(void){
    if(!stateErr5){
        stateErr5 = true;
        btn5->setStyleSheet("color: red");
    }else{
        stateErr5 = false;
        btn5->setStyleSheet(style);
    }
}

void FormCorrection::btn6_clicked(void){
    if(!stateErr6){
        stateErr6 = true;
        btn6->setStyleSheet("color: red");
    }else{
        stateErr6 = false;
        btn6->setStyleSheet(style);
    }
}

void FormCorrection::btn7_clicked(void){
    if(!stateErr7){
        stateErr7 = true;
        btn7->setStyleSheet("color: red");
    }else{
        stateErr7 = false;
        btn7->setStyleSheet(style);
    }
}

void FormCorrection::btn8_clicked(void){
    if(!stateErr8){
        stateErr8 = true;
        btn8->setStyleSheet("color: red");
    }else{
        stateErr8 = false;
        btn8->setStyleSheet(style);
    }
}

void FormCorrection::btn9_clicked(void){
    if(!stateErr9){
        stateErr9 = true;
        btn9->setStyleSheet("color: red");
    }else{
        stateErr9 = false;
        btn9->setStyleSheet(style);
    }
}

void FormCorrection::btn10_clicked(void){
    if(!stateErr10){
        stateErr10 = true;
        btn10->setStyleSheet("color: red");
    }else{
        stateErr10 = false;
        btn10->setStyleSheet(style);
    }
}

void FormCorrection::btn11_clicked(void){
    if(!stateErr11){
        stateErr11 = true;
        btn11->setStyleSheet("color: red");
    }else{
        stateErr11 = false;
        btn11->setStyleSheet(style);
    }
}

void FormCorrection::btn12_clicked(void){
    if(!stateErr12){
        stateErr12 = true;
        btn12->setStyleSheet("color: red");
    }else{
        stateErr12 = false;
        btn12->setStyleSheet(style);
    }
}

void FormCorrection::btn13_clicked(void){
    if(!stateErr13){
        stateErr13 = true;
        btn13->setStyleSheet("color: red");
    }else{
        stateErr13 = false;
        btn13->setStyleSheet(style);
    }
}

void FormCorrection::btn14_clicked(void){
    if(!stateErr14){
        stateErr14 = true;
        btn14->setStyleSheet("color: red");
    }else{
        stateErr14 = false;
        btn14->setStyleSheet(style);
    }
}

void FormCorrection::btn15_clicked(void){
    if(!stateErr15){
        stateErr15 = true;
        btn15->setStyleSheet("color: red");
    }else{
        stateErr15 = false;
        btn15->setStyleSheet(style);
    }
}

void FormCorrection::btnPlus_clicked(void){
    if(!statePlusOne){
        statePlusOne = true;
        btnPlus->setStyleSheet("color: red");
    }else{
        statePlusOne = false;
        btnPlus->setStyleSheet(style);
    }
}

void FormCorrection::btnZero_clicked(void){
    if(!stateZero){
        stateZero = true;
        btnZero->setStyleSheet("color: red");
    }else{
        stateZero = false;
        btnZero->setStyleSheet(style);
    }
}

void FormCorrection::btnOk_clicked(void){
    parent_led->setStyleSheet("background-color:#ffffff;");
    QList<QString> error;
    float rate = 10.0;
    if(stateErr1){
        error.append("1");
        rate -= 0.5;
    }
    if(stateErr2){
        error.append("2");
        rate -= 0.5;
    }
    if(stateErr3){
        error.append("3");
        rate -= 0.5;
    }
    if(stateErr4){
        error.append("4");
        rate -= 0.5;
    }
    if(stateErr5){
        error.append("5");
        rate -=0.5;
    }
    if(stateErr6){
        error.append("6");
        rate -= 1.0;
    }
    if(stateErr7){
        error.append("7");
        rate -= 1.0;
    }
    if(stateErr8){
        error.append("8");
        rate -= 1.0;
    }
    if(stateErr9){
        error.append("9");
        rate -= 1.0;
    }
    if(stateErr10){
        error.append("10");
        rate -= 1.0;
    }
    if(stateErr11){
        error.append("11");
        rate -= 2.0;
    }
    if(stateErr12){
        error.append("12");
        rate -= 2.0;
    }
    if(stateErr13){
        error.append("13");
        rate -= 2.0;
    }
    if(stateErr14){
        error.append("14");
        rate -= 2.0;
    }
    if(stateErr15){
        error.append("15");
        rate -= 2.0;
    }
    if(statePlusOne){
        error.append("+1");
        rate += 1.0;
    }
    if(stateZero){
        error.append("0");
        rate = 0.0;
    }
    if(rate < 0) rate = 0.0;
    QString sError = "";
    if(error.length() > 0){
        foreach (QString each, error)
            sError = sError + each + ", ";
        sError.remove(sError.length() - 2, 2);
    }
    parent_led->setText(sError);                //запись ошибок
    objRate->setText(QString::number(rate));    // запись оценок

    // чтение всех оценок в вопросе в список lSum
    float summa = 0.0;
    QList<float> lSum;
    for(int i=1; i<6; i++){
        QString tSum = parent_form->findChild<QLabel*>
                ("lbRate" + parent_led->objectName()[parent_led->objectName().length() - 2]
                 + QString::number(i))->text();
        /*
        r = reg.findall(tSum)
            l = ''
            for i in r:
                l = l + i
         */
        //lSum.append(float(l))
        //if(rate.contains("<")){
        int pos = reg.indexIn(tSum);
        QString r = reg.cap(0);
            //l.append(r);
        //}else
            //l.append(rate);
        lSum.append(r.toFloat());
    }
        // определение минимальной и максимальной оценки
    int index_min = 0;
    int index_max = 0;
    float minRate = lSum[index_min];
    float maxRate = lSum[index_max];
    for(int i=0; i<5; i++){
        if(lSum[i] < minRate){
            minRate = lSum[i];
            index_min = i;
        }
        if(lSum[i] >= maxRate){
            maxRate = lSum[i];
            index_max = i;
        }
    }
        // запись всех оценок вопроса и определение суммы
    for(int i=0; i<5; i++){
        QLineEdit* led = p->findChild<QLineEdit*>("led" + QString::number(i + 1));
        Ball* lblRate;
        if(((FormMain*)p)->flag_mode == 0)
            lblRate = ((FormMain*)p)->fr->findChild<Ball*>("rate_" + QString::number(i + 1));
        else
            lblRate = ((FormMain*)p)->fr2->findChild<Ball*>("rate_" + QString::number(i + 1));

        if(i == index_min){
            parent_form->findChild<QLabel*>("lbRate" +
                parent_led->objectName()[parent_led->objectName().length() - 2] +
                                                   QString::number(i + 1))->setText("<s>" +
                                                   QString::number(lSum[index_min]) + "</s>");
            led->setText(QString::number(lSum[index_min]));
            led->setStyleSheet("background-color:#0000ff;");

            lblRate->setText(QString::number(lSum[index_min]));
            QFile file("style_min_max.qss");
            file.open(QIODevice::ReadOnly);
            QString Style = file.readAll();
            lblRate->setStyleSheet(Style);
            file.close();
        }else if(i == index_max){
            parent_form->findChild<QLabel*>("lbRate" +
                parent_led->objectName()[parent_led->objectName().length() - 2] +
                                                   QString::number(i + 1))->setText("<s>" +
                                                   QString::number(lSum[index_max]) + "</s>");
            led->setText(QString::number(lSum[index_max]));
            led->setStyleSheet("background-color:#ff0000;");

            lblRate->setText(QString::number(lSum[index_max]));
            QFile file("style_min_max.qss");
            file.open(QIODevice::ReadOnly);
            QString Style = file.readAll();
            lblRate->setStyleSheet(Style);
            file.close();
        }else{
            parent_form->findChild<QLabel*>("lbRate" +
                parent_led->objectName()[parent_led->objectName().length() - 2] +
                                  QString::number(i + 1))->setText(QString::number(lSum[i]));

            led->setText(QString::number(lSum[i]));
            led->setStyleSheet("background-color:#ffffff;");

            lblRate->setText(QString::number(lSum[i]));
            QFile file("style_min_max.qss");
            file.open(QIODevice::ReadOnly);
            QString Style = file.readAll();
            lblRate->setStyleSheet(Style);
            file.close();
            summa += float(lSum[i]);
        }

    }

    objSum->setText(QString::number(summa));
    ((FormMain*)p)->LBL_SUM->setText(QString::number(summa));
    if(((FormMain*)p)->flag_mode == 0) ((FormMain*)p)->fr->rate_sum->setText(QString::number(summa));
    else ((FormMain*)p)->fr2->rate_sum->setText(QString::number(summa));
    // определение и запись общей суммы
    float totalSum = 0.0;
    for(int i=1; i<6; i++){
        QString tSum = parent_form->findChild<QLabel*>("lblSum" + QString::number(i))->text();
        bool ok;
        float f = tSum.toFloat(&ok);
        if(ok == false)
            f = 0.0;
        totalSum += f;
    }
    ((Errors*)parent_form)->LBLTOTALSUM->setText(QString::number(totalSum));
    ((FormMain*)p)->LBL_TOTAL->setText(QString::number(totalSum));
    if(((FormMain*)p)->flag_mode == 0){
        ((FormMain*)p)->fr->rate_total->setText(QString::number(totalSum));
        ((FormMain*)p)->total_sum = totalSum;

    }else{
        if(((FormMain*)p)->current_color == "red"){
            ((FormMain*)p)->fr2->rate_total_red->setText(QString::number(totalSum));
            ((FormMain*)p)->total_sum_red = totalSum;
        }else{
            ((FormMain*)p)->fr2->rate_total_blue->setText(QString::number(totalSum));
            ((FormMain*)p)->total_sum_blue = totalSum;
        }
    }
    record_to_baza();
    close();
}

void FormCorrection::btnCancel_clicked(void){
    parent_led->setStyleSheet("background-color:#ffffff;");
    close();
}

void FormCorrection::record_to_baza(void){
    QList<QString> l;
    l.append(parent_led->text());   // ошибки текущего вопроса и корректируемого судьи
    QString reffery = parent_led->objectName()[parent_led->objectName().length() - 1] + "_";   // текущий судья
    // добавление судейских оценок
    for(int i=1;i<6; i++){
        QString rate = parent_form->findChild<QLabel*>("lbRate" +
            parent_led->objectName()[parent_led->objectName().length() - 2] +
                                                            QString::number(i))->text();
        if(rate.contains("<")){
            int pos = reg.indexIn(rate);
            QString r = "N" + reg.cap(0);
            l.append(r);
        }else
            l.append(rate);
    }
    l.append(((FormMain*)p)->LBL_SUM->text());                      // добавление суммы вопроса
    l.append(((Errors*)parent_form)->LBLTOTALSUM->text());        // добавление общей суммы
    // добавление id корректируемой записи и вычисление текуще приема
    QString priem;
    if(((FormMain*)p)->flag_mode == 0){
        l.append(QString::number(((FormMain*)p)->id_rates));
        priem = QString::number((((FormMain*)p)->flag_priem));
    }else{
        if(((FormMain*)p)->current_color == "red")
            l.append(QString::number(((FormMain*)p)->id_rates_red));
        else
            l.append(QString::number(((FormMain*)p)->id_rates_blue));
        priem = QString::number(static_cast<int>((float)(((FormMain*)p)->flag_priem) / 2.0 + 0.5));
    }
    QString sql = "UPDATE errors_and_rates SET "
                  "errors" + reffery + priem + " = '%1', "
                  "rate1_" + priem + " = '%2', rate2_" + priem + " = '%3', rate3_" +  priem + " = '%4', "
                  "rate4_" + priem + "   = '%5', rate5_" + priem + "   = '%6', "
                  "sum" + priem + "      = '%7', total     = '%8' "
                  "WHERE id = '%9' ";
    sql = sql.arg(l[0])
             .arg(l[1])
             .arg(l[2])
             .arg(l[3])
             .arg(l[4])
             .arg(l[5])
             .arg(l[6])
             .arg(l[7])
             .arg(l[8]);
    qDebug() << "sql correction" << sql;
    QSqlQuery query;
    if(!query.exec(sql))
        qDebug() << "correction error";
}

