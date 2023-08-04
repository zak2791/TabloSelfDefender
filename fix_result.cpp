#include "FormMain.h"
#include <QSqlQuery>
/////////////////////////////////////////////////////////////////////////////
//                 фиксация ошибок в текущем вопросе                       //
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                                 F1                                      //
/////////////////////////////////////////////////////////////////////////////
void F1(FormMain* p){
    p->fr2->rate_1->setText(p->Led1->text());
    p->fr2->rate_2->setText(p->Led2->text());
    p->fr2->rate_3->setText(p->Led3->text());
    p->fr2->rate_4->setText(p->Led4->text());
    p->fr2->rate_5->setText(p->Led5->text());

    p->enable_correct = true;

    QString s;
    if(p->current_color == "red"){
        p->total_sum_red += (p->Lbl_sum->text()).toFloat();
        s.setNum(p->total_sum_red);
        p->Lbl_total_red->setText(s);
        p->fr2->rate_total_red->setRate(s);
    }else{
        p->total_sum_blue += (p->Lbl_sum->text()).toFloat();
        s.setNum(p->total_sum_blue);
        p->Lbl_total_blue->setText(s);
        p->fr2->rate_total_blue->setRate(s);
    }

    QList<QString> l;
    int index = int((float)p->flag_priem / 2.0 + 0.5) - 1;

    s = p->lref1_red_blue[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref2_red_blue[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref3_red_blue[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref4_red_blue[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref5_red_blue[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    for(int count_y=1;count_y<6;count_y++){
        QLineEdit* led = p->findChild<QLineEdit*>("led" + QString::number(count_y));
        QString sStyle = led->styleSheet().split(" ")[2];
        sStyle.truncate(sStyle.length() - 2);
        if(sStyle != "white")
            l.append("N" + led->text());
        else
            l.append(led->text());
    }

    l.append(p->Lbl_sum->text());
    if(p->current_color == "red"){
        l.append(p->Lbl_total_red->text());
        l.append(QString::number(p->id_rates_red));
        p->id_correct = p->id_rates_red;
    }else{
        l.append(p->Lbl_total_blue->text());
        l.append(QString::number(p->id_rates_blue));
        p->id_correct = p->id_rates_blue;
    }
    if(p->flag_priem < 10)
        p->Btn_next->setEnabled(true);
////////////////////////////////////////////////////////////////////////////////////////////////
    QString priem = QString::number(int((float)(p->flag_priem) / 2.0 + 0.5));
    QString sql =   "UPDATE errors_and_rates SET "
                    "errors1_" + priem + " = '%1', errors2_" + priem + " = '%2', "
                    "errors3_" + priem + " = '%3', errors4_" + priem + " = '%4', errors5_" + priem + " = '%5', "
                    "rate1_"   + priem + "   = '%6', rate2_" + priem + "   = '%7', rate3_" + priem + "   = '%8', "
                    "rate4_"   + priem + "   = '%9', rate5_" + priem + "   = '%10', "
                    "sum"      + priem + "      = '%11', total     = '%12' "
                    "WHERE id = '%13' ";

    sql = sql.arg(l[0])
             .arg(l[1])
             .arg(l[2])
             .arg(l[3])
             .arg(l[4])
             .arg(l[5])
             .arg(l[6])
             .arg(l[7])
             .arg(l[8])
             .arg(l[9])
             .arg(l[10])
             .arg(l[11])
             .arg(l[12]);
    QSqlQuery query;
    if(!query.exec(sql)) qDebug() << "error insert rate";
}

/////////////////////////////////////////////////////////////////////////////
//                                 F2                                      //
/////////////////////////////////////////////////////////////////////////////
void F2(FormMain* p){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(((FormMain*)p)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных F2!");
        msgBox.exec();
        return;
    }
    p->fr->rate_1->setText(p->Led1->text());
    p->fr->rate_2->setText(p->Led2->text());
    p->fr->rate_3->setText(p->Led3->text());
    p->fr->rate_4->setText(p->Led4->text());
    p->fr->rate_5->setText(p->Led5->text());

    p->total_sum += (p->Lbl_sum->text()).toFloat();
    QString s;
    s.setNum(p->total_sum);
    p->LBL_TOTAL->setText(s);
    s.setNum(p->total_sum);
    p->fr->rate_total->setRate(s);

    p->enable_correct = true;

    ///////////////////////////////////////////////////
    QList<QString> l;
    int index = p->flag_priem - 1;

    s = p->lref1[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref2[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref3[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref4[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    s = p->lref5[index];
    s.truncate(s.length() - 1);
    s.remove(0,1);
    l.append(s);

    for(int count_y=1;count_y<6;count_y++){
        QLineEdit* led = p->findChild<QLineEdit*>("led" + QString::number(count_y));
        QString  sStyle = led->styleSheet();
        sStyle = led->styleSheet().split(" ")[2];
        sStyle.truncate(sStyle.length() - 2);

        if(sStyle != "white")
            l.append('N' + led->text());
        else
            l.append(led->text());
    }
    l.append(p->Lbl_sum->text());
    l.append(p->LBL_TOTAL->text());
    l.append(QString::number(p->id_rates));

    if(p->flag_priem < 5)
        p->Btn_next->setEnabled(true);
    ////////////////////////////////////////////////////////////////////////////////////////////////

    QString priem = QString::number(p->flag_priem);
    QString sql = "UPDATE errors_and_rates SET "
                  "errors1_" + priem + " = '%1', errors2_" + priem + " = '%2', errors3_" + priem + " = '%3', "
                  "errors4_" + priem + " = '%4', errors5_" + priem + " = '%5', "
                  "rate1_" + priem + "   = '%6', rate2_" + priem + "   = '%7', rate3_" + priem + "   = '%8', "
                  "rate4_" + priem + "   = '%9', rate5_" + priem + "   = '%10', "
                  "sum" + priem + "      = '%11', total     = '%12'"
                  "WHERE id = '%13' ";

    sql = sql.arg(l[0])
             .arg(l[1])
             .arg(l[2])
             .arg(l[3])
             .arg(l[4])
             .arg(l[5])
             .arg(l[6])
             .arg(l[7])
             .arg(l[8])
             .arg(l[9])
             .arg(l[10])
             .arg(l[11])
             .arg(l[12]);
    QSqlQuery query;
    if(!query.exec(sql)) qDebug() << "error insert rate";
    m_db.close();
}

/////////////////////////////////////////////////////////////////////////////
//                  основная функция фиксации результата                   //
/////////////////////////////////////////////////////////////////////////////
void fix_result(FormMain* p){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(((FormMain*)p)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных fix_result!");
        msgBox.exec();
        return;
    }
    if(p->flag_priem == 1){       // если приём № 1
        if(p->current_mode == -1){
         //////////////////////////////////////////////////////
         //       запись в таблицу rounds значения mode      //
         //////////////////////////////////////////////////////
            QString sql_mode = "UPDATE rounds SET mode = " +  QString::number(p->flag_mode) +
                               " WHERE id = " + QString::number(p->id_round);


            QSqlQuery query;
            if(!query.exec(sql_mode)){
                qDebug() << "error insert rate";
                return;
            }
            p->current_mode = p->flag_mode;
            p->GroupBox->setEnabled(false);
        }
        //////////////////////////////////////////////////////
        //         создание новой записи в таблице          //
        //////////////////////////////////////////////////////
        if(p->current_mode == 0){
            QString sql_sportsmens = "SELECT id FROM sportsmens WHERE name = '" +
                                        p->Lbl_name->text().split(';')[0] +
                                        "'";
            QSqlQuery query;
            query.exec(sql_sportsmens);
            query.next();
            QString IDS = query.value(0).toString();                                               // id спортсмена
            // вставка записи в таблицу ошибок
            QString sql_add_rates = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
            sql_add_rates = sql_add_rates.arg(IDS).arg(p->id_round);
            if(!query.exec(sql_add_rates)){
                qDebug() << "error insert rate";
                return;
            }

            QString sql = "SELECT id FROM errors_and_rates WHERE id_sportsmen = '%1' and id_round = '%2'";
            sql = sql.arg(IDS).arg(p->id_round);
            query.exec(sql);
            query.next();
            p->id_rates = query.value(0).toInt();
            F2(p);
        }else{                   //current_mode = 1
            /////////////////////////////////////////////////////////////
            // создание новой записи в таблице для красного спортсмена //
            /////////////////////////////////////////////////////////////
            QString sql_sportsmens = "SELECT id FROM sportsmens WHERE name = '" +
                                        p->Lbl_name_red->text().split(';')[0] +
                                        "'";
            QSqlQuery query;
            query.exec(sql_sportsmens);
            query.next();
            QString IDS = query.value(0).toString();    // id спортсмена
            // вставка записи в таблицу ошибок
            QString sql_add_rates = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
            sql_add_rates = sql_add_rates.arg(IDS).arg(p->id_round);
            if(!query.exec(sql_add_rates)){
                qDebug() << "error insert rate";
                return;
            }
            QString sql = "SELECT id FROM errors_and_rates WHERE id_sportsmen = '%1' and id_round = '%2'";
            sql = sql.arg(IDS).arg(p->id_round);
            query.exec(sql);
            query.next();
            p->id_rates_red = query.value(0).toInt();
            /////////////////////////////////////////////////////////////
            //  создание новой записи в таблице для синего спортсмена  //
            /////////////////////////////////////////////////////////////
            sql_sportsmens = "SELECT id FROM sportsmens WHERE name = '" +
                                p->Lbl_name_blue->text().split(';')[0] +
                                "'";
            query.exec(sql_sportsmens);
            query.next();
            IDS = query.value(0).toString();        // id спортсмена
            // вставка записи в таблицу ошибок
            sql_add_rates = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
            sql_add_rates = sql_add_rates.arg(IDS).arg(p->id_round);
            if(!query.exec(sql_add_rates)){
                qDebug() << "error insert rate";
                return;
            }

            sql = "SELECT id FROM errors_and_rates WHERE id_sportsmen = '%1' and id_round = '%2'";
            sql = sql.arg(IDS).arg(p->id_round);
            query.exec(sql);
            query.next();
            p->id_rates_blue = query.value(0).toInt();
            F1(p);
        }

    }else{           // если прием № 2, 3, 4 или 5
        if(p->current_mode == 0)
            F2(p);
        else
            F1(p);
    }
    m_db.close();
}

int dialog(FormMain* p){
        QDialog* dlg = new QDialog();
        dlg->setWindowFlags(Qt::SplashScreen);
        QPushButton btnOk("OK");
        QPushButton btnCancel("Cancel");

        QLabel lbl("Вы уверены?");

        lbl.setAlignment(Qt::AlignHCenter);
        QHBoxLayout* hbox = new  QHBoxLayout();
        QVBoxLayout* vbox = new QVBoxLayout();
        hbox->addWidget(&btnOk);
        hbox->addWidget(&btnCancel);
        vbox->addWidget(&lbl);
        vbox->addLayout(hbox);
        dlg->setLayout(vbox);
        p->connect(&btnOk, SIGNAL(clicked()), dlg, SLOT(accept()));
        p->connect(&btnCancel, SIGNAL(clicked()), dlg, SLOT(reject()));
        int ret = dlg->exec();
        return ret;
}
