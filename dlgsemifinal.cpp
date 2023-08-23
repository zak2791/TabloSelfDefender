#include "dlgsemifinal.h"
#include "ui_dlgsemifinal.h"

#include <QSqlDatabase>
#include <QMessageBox>

dlgSemiFinal::dlgSemiFinal(QWidget *parent, int mode) :
    QDialog(parent),
    ui(new Ui::dlgSemiFinal)
{
    ui->setupUi(this);

    LblOne = ui->lblOne;
    LblTwo = ui->lblTwo;

    RbOne = ui->rbOne;
    RbTwo = ui->rbTwo;

    fm = static_cast<FormMain*>(parent);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Выберите пару спортсменов");

    QSqlDatabase m_db;
    QMessageBox msgBox;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(fm->currentDataBase);
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных (btn_semiFinal_clicked)!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    QString sql = "SELECT sp.name, sp.region FROM sportsmens sp JOIN errors_and_rates err ON sp.id=err.id_sportsmen WHERE err.id_round = "
                  "(SELECT id FROM rounds WHERE age = '%1' and weight = '%2' and mode = 2) ORDER BY err.id";

    sql =         "SELECT sp.name, sp.region FROM sportsmens sp JOIN errors_and_rates err ON sp.id=err.id_sportsmen "
                  "JOIN rounds rnd "
                  "ON rnd.id=err.id_round WHERE rnd.age = '%1' and rnd.weight = '%2' and rnd.mode = 2 ORDER BY err.id";

    if(mode == 3){
        sql =         "SELECT sp.name, sp.region FROM sportsmens sp JOIN errors_and_rates err ON sp.id=err.id_sportsmen "
                      "JOIN rounds rnd "
                      "ON rnd.id=err.id_round WHERE rnd.age = '%1' and rnd.weight = '%2' and rnd.mode = 3 ORDER BY err.id";
        RbOne->setText("За 3 место");
        RbTwo->setText("Финал");
    }



    if(!query.exec(sql.arg(fm->CmbAge->currentText(), fm->CmbWeight->currentText()))){
        qDebug()<<"error = "<<query.lastError();
        return;
    }
    int count = 0;  // определение числа спортсменов в круге
    while(query.next())
        count++;

    if(count == 4){
        query.first();
        QString one = query.value(0).toString() + ";" + query.value(1).toString();
        query.next();
        QString two = query.value(0).toString() + ";" + query.value(1).toString();
        LblOne->setText(one + "\n" + two);
        query.next();
        one = query.value(0).toString() + ";" + query.value(1).toString();
        query.next();
        two = query.value(0).toString() + ";" + query.value(1).toString();
        LblTwo->setText(one + "\n" + two);
    }
    else if(count == 3){
        query.first();
        QString one = query.value(0).toString() + ";" + query.value(1).toString();
        LblOne->setText(one);
        query.next();
        one = query.value(0).toString() + ";" + query.value(1).toString();
        query.next();
        QString two = query.value(0).toString() + ";" + query.value(1).toString();
        LblTwo->setText(one + "\n" + two);
        LblOne->setEnabled(false);
        RbOne->setEnabled(false);
    }
    else if(count == 2){
        query.first();
        QString one = query.value(0).toString() + ";" + query.value(1).toString();
        query.next();
        QString two = query.value(0).toString() + ";" + query.value(1).toString();
        LblTwo->setText(one + "\n" + two);
        LblOne->setText("");
    }

    sql = "SELECT total FROM errors_and_rates err JOIN rounds rnd ON err.id_round=rnd.id "
          "WHERE rnd.age = '%1' and rnd.weight = '%2' and rnd.mode = '%3' ORDER BY err.id";
    sql = sql.arg(fm->CmbAge->currentText(), fm->CmbWeight->currentText(), QString::number(mode));
    if(!query.exec(sql)) qDebug()<<"error query.exec(sql) = "<<query.lastError();
    else{
        if(query.next()){
            if(!query.value(0).isNull()){
                LblOne->setEnabled(false);
                RbOne->setEnabled(false);
            }
        }
        if(query.next()){
            if(!query.value(0).isNull()){
                LblOne->setEnabled(false);
                RbOne->setEnabled(false);
            }
        }
        if(query.next()){
            if(!query.value(0).isNull()){
                LblTwo->setEnabled(false);
                RbTwo->setEnabled(false);
            }
        }
        if(query.next()){
            if(!query.value(0).isNull()){
                LblTwo->setEnabled(false);
                RbTwo->setEnabled(false);
            }
        }
    }

    if(LblOne->text() == ""){
        LblOne->setEnabled(false);
        RbOne->setEnabled(false);
    }

    connect(ui->rbOne, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    connect(ui->rbTwo, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(btnOk_clicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(btnCancel_clicked()));

}

void dlgSemiFinal::btn_clicked(){
    if(sender()->objectName() == "rbOne"){
        red = LblOne->text().split("\n")[0];
        blue = LblOne->text().split("\n")[1];
    }
    else{
        red = LblTwo->text().split("\n")[0];
        blue = LblTwo->text().split("\n")[1];
    }
}

void dlgSemiFinal::btnOk_clicked(){
    if(red == "" || blue == ""){
        close();
        return;
    }
    fm->fr2->rate_total_red->setText("");
    fm->fr2->rate_total_blue->setText("");

    fm->fr2->rate_sum->setText("");
    fm->fr2->count_priem->setText("");

    fm->fr2->rate_1->setText("");
    fm->fr2->rate_2->setText("");
    fm->fr2->rate_3->setText("");
    fm->fr2->rate_4->setText("");
    fm->fr2->rate_5->setText("");
    fm->fr2->rate_1->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                  "border-style: solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_2->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_3->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_4->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_5->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");

    family_set();

    fm->GroupBox_2->setEnabled(true);
    fm->RbBlue->setChecked(true);

    fm->Btn_next->setEnabled(true);
    fm->GroupBox->setEnabled(false);
    fm->GroupBox->setEnabled(false);
    fm->Cmb_round->setEnabled(false);
    fm->CmbAge->setEnabled(false);
    fm->CmbWeight->setEnabled(false);
    fm->Btn_new->setEnabled(false);
    fm->BtnFinal->setEnabled(false);
    fm->BtnSemiFinal->setEnabled(false);
    fm->mainwin->menuBar()->setEnabled(false);

    close();

}

void dlgSemiFinal::btnCancel_clicked(){
    fm->fr2->rate_total_red->setText("");
    fm->fr2->rate_total_blue->setText("");

    fm->fr2->rate_sum->setText("");
    fm->fr2->count_priem->setText("");

    fm->fr2->rate_1->setText("");
    fm->fr2->rate_2->setText("");
    fm->fr2->rate_3->setText("");
    fm->fr2->rate_4->setText("");
    fm->fr2->rate_5->setText("");
    fm->fr2->rate_1->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                  "border-style: solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_2->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_3->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_4->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    fm->fr2->rate_5->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");

    fm->Btn_next->setEnabled(false);
    fm->GroupBox->setEnabled(false);
    fm->Cmb_round->setEnabled(true);
    fm->CmbAge->setEnabled(true);
    fm->CmbWeight->setEnabled(true);
    fm->Btn_new->setEnabled(true);
    fm->BtnFinal->setEnabled(true);
    fm->BtnSemiFinal->setEnabled(true);
    fm->mainwin->menuBar()->setEnabled(true);

    fm->fr2->name_red->Text("");
    fm->fr2->region_red->Text("");
    fm->fr2->lbl_flag_red->clear();

    fm->fr2->name_blue->Text("");
    fm->fr2->region_blue->Text("");
    fm->fr2->lbl_flag_blue->clear();

    fm->Lbl_name_red->setText("");
    fm->Lbl_name_blue->setText("");

    close();
}

void dlgSemiFinal::family_set(){
    fm->Lbl_name_red->setText(red);
    fm->fr2->name_red->Text(red.split(";")[0].toUpper());
    fm->fr2->region_red->Text(red.split(";")[1].toUpper());
    QString path = "flags/";
    path = path + red.split(";")[1] + ".png";
    QFile file(path);
    if(file.exists()){
        fm->flag_red.load(path);
        fm->fr2->lbl_flag_red->setPixmap(fm->flag_red.scaled(fm->fr2->lbl_flag_red->size()));

    }else
        fm->fr2->lbl_flag_red->clear();

    fm->Lbl_name_blue->setText(blue);
    fm->fr2->name_blue->Text(blue.split(";")[0].toUpper());
    fm->fr2->region_blue->Text(blue.split(";")[1].toUpper());
    path = "flags/";
    path = path + blue.split(";")[1] + ".png";
    QFile file2(path);
    if(file2.exists()){
        fm->flag_red.load(path);
        fm->fr2->lbl_flag_blue->setPixmap(fm->flag_blue.scaled(fm->fr2->lbl_flag_blue->size()));
    }else
        fm->fr2->lbl_flag_blue->clear();

}

dlgSemiFinal::~dlgSemiFinal()
{
    delete ui;
}
