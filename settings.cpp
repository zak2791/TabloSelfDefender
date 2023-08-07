#include "settings.h"

#include "FormMain.h"

Settings::Settings(QWidget *parent):QWidget(){

    setupUi(this);

    RbnSeparate = rbnSeparate;
    RbnTogether = rbnTogether;
    RbEng = rbEng;



    ip = "";
    code = "";
    id = "";
    regFail = true;

    pW = parent;

    RbOne = rbnOne;

    connect(btnReg, SIGNAL(clicked(bool)), SLOT(registration()));
    connect(cmb_main_judge, SIGNAL(activated(int)), SLOT(judge_main(int)));
    connect(cmb_main_secretary, SIGNAL(activated(int)), SLOT(judge_secretary(int)));
    connect(cmb_judge1, SIGNAL(activated(int)), SLOT(judge_1(int)));
    connect(cmb_judge2, SIGNAL(activated(int)), SLOT(judge_2(int)));
    connect(cmb_judge3, SIGNAL(activated(int)), SLOT(judge_3(int)));
    connect(cmb_judge4, SIGNAL(activated(int)), SLOT(judge_4(int)));
    connect(cmb_judge5, SIGNAL(activated(int)), SLOT(judge_5(int)));

    connect(cmb_ts, SIGNAL(activated(int)), SLOT(ts(int)));

    connect(rbnSeparate, SIGNAL(toggled(bool)), SLOT(separate(bool)));
    connect(rbnOne, SIGNAL(toggled(bool)), SLOT(team_referees(bool)));
    connect(rbnTwo, SIGNAL(toggled(bool)), SLOT(team_referees(bool)));
    connect(rbRus, SIGNAL(toggled(bool)), SLOT(rus_to_eng(bool)));
    connect(cbFlag, SIGNAL(stateChanged(int)), SLOT(showFlag(int)));
    connect(btnReg2, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg3, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg4, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg5, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReset, SIGNAL(clicked(bool)), SLOT(registrationReset()));

    leCode->setText("1234");

}

void Settings::showEvent(QShowEvent*){
    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    if(settings.value("pult2").toString() != "0")
        btnReg2->setStyleSheet("color: #ff0000");
    else
        btnReg2->setStyleSheet("color: #000000");
    if(settings.value("pult3").toString() != "0")
        btnReg3->setStyleSheet("color: #ff0000");
    else
        btnReg3->setStyleSheet("color: #000000");
    if(settings.value("pult4").toString() != "0")
        btnReg4->setStyleSheet("color: #ff0000");
    else
        btnReg4->setStyleSheet("color: #000000");
    if(settings.value("pult5").toString() != "0")
        btnReg5->setStyleSheet("color: #ff0000");
    else
        btnReg5->setStyleSheet("color: #000000");
    settings.endGroup();



    setCombo();

}

void Settings::setCombo(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных showEvent!");
        msgBox.exec();
        return;
    }

    QStringList l;
    foreach(QStringList sL, static_cast<FormMain*>(pW)->getReferees()){
        l.append(sL.at(1));
    }
    //mdl.removeRows(0, mdl.rowCount());

    mdl.setStringList(l);
    mdl.insertRows(0,1);        //вставка пустой строки
    mdl.sort(0);
    cmb_main_secretary->setModel(&mdl);
    cmb_main_judge->setModel(&mdl);
    cmb_judge1->setModel(&mdl);
    cmb_judge2->setModel(&mdl);
    cmb_judge3->setModel(&mdl);
    cmb_judge4->setModel(&mdl);
    cmb_judge5->setModel(&mdl);
    cmb_ts->setModel(&mdl);

    QSqlQuery query;

    cmb_main_judge->setCurrentIndex(0);
    cmb_main_secretary->setCurrentIndex(0);
    cmb_judge1->setCurrentIndex(0);
    cmb_judge2->setCurrentIndex(0);
    cmb_judge3->setCurrentIndex(0);
    cmb_judge4->setCurrentIndex(0);
    cmb_judge5->setCurrentIndex(0);
    cmb_ts->setCurrentIndex(0);

    if(RbOne->isChecked()){
        query.exec("SELECT name FROM referee WHERE role1 = 1");
        if(query.next())
            cmb_main_judge->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 2");
        if(query.next())
            cmb_main_secretary->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 3");
        if(query.next())
            cmb_judge1->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 4");
        if(query.next())
            cmb_judge2->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 5");
        if(query.next())
            cmb_judge3->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 6");
        if(query.next())
            cmb_judge4->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 7");
        if(query.next())
            cmb_judge5->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role1 = 8");
        if(query.next())
            cmb_ts->setCurrentText(query.value(0).toString());
    }else{
        query.exec("SELECT name FROM referee WHERE role2 = 1");
        if(query.next())
            cmb_main_judge->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 2");
        if(query.next())
            cmb_main_secretary->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 3");
        if(query.next())
            cmb_judge1->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 4");
        if(query.next())
            cmb_judge2->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 5");
        if(query.next())
            cmb_judge3->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 6");
        if(query.next())
            cmb_judge4->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 7");
        if(query.next())
            cmb_judge5->setCurrentText(query.value(0).toString());
        query.exec("SELECT name FROM referee WHERE role2 = 8");
        if(query.next())
            cmb_ts->setCurrentText(query.value(0).toString());
    }

    static_cast<FormMain*>(pW)->main_refery = cmb_main_judge->currentText();
    static_cast<FormMain*>(pW)->main_secretary = cmb_main_secretary->currentText();
    static_cast<FormMain*>(pW)->judge1 = cmb_judge1->currentText();
    static_cast<FormMain*>(pW)->judge2 = cmb_judge2->currentText();
    static_cast<FormMain*>(pW)->judge3 = cmb_judge3->currentText();
    static_cast<FormMain*>(pW)->judge4 = cmb_judge4->currentText();
    static_cast<FormMain*>(pW)->judge5 = cmb_judge5->currentText();
    static_cast<FormMain*>(pW)->ts = cmb_ts->currentText();

    m_db.close();
}

void Settings::registration(void){
    FormMain* p = static_cast<FormMain*>(pW);
    //Registration* reg;
    //reg = sock_registration_udp.thread();
    qDebug()<<"p->ret_mat = "<<p->ret_mat;
    if(p->ret_mat)
        reg = new Registration(4000, this);
    else
        reg = new Registration(4001, this);

    connect(reg, SIGNAL(registrationSuccesfull(QString, QString, QString)), this,
                 SLOT(registrationSuccesfull(QString, QString, QString)));
    connect(reg, SIGNAL(registrationStart(QString)),
                 SLOT(registrationStart(QString)));
    connect(reg, SIGNAL(registrationFailed()),
                 SLOT(registrationFailed()));


    btnReg->setEnabled(false);
    btnReset->setEnabled(false);
}

void Settings::judge_main(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_1!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;

    query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 1");
    query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 1");

    query.exec("UPDATE referee SET role1 = 1 WHERE name = '" + cmb_main_judge->currentText() + "'");
    query.exec("UPDATE referee SET role2 = 1 WHERE name = '" + cmb_main_judge->currentText() + "'");

    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();
}

void Settings::judge_secretary(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_1!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;

    query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 2");
    query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 2");

    query.exec("UPDATE referee SET role1 = 2 WHERE name = '" + cmb_main_secretary->currentText() + "'");
    query.exec("UPDATE referee SET role2 = 2 WHERE name = '" + cmb_main_secretary->currentText() + "'");

    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();
}

void Settings::judge_1(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_1!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 3");
        query.exec("UPDATE referee SET role1 = 3 WHERE name = '" + cmb_judge1->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 3");
        query.exec("UPDATE referee SET role2 = 3 WHERE name = '" + cmb_judge1->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();


//    if(cbFlag->isChecked())
//        flag_set(p->fr->judge_flag1, cmb_judge1->itemText(index));
}

void Settings::judge_2(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_2!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 4");
        query.exec("UPDATE referee SET role1 = 4 WHERE name = '" + cmb_judge2->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 4");
        query.exec("UPDATE referee SET role2 = 4 WHERE name = '" + cmb_judge2->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();

//    if(cbFlag->isChecked())
//        flag_set(p->fr->judge_flag2, cmb_judge2->itemText(index));
}

void Settings::judge_3(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_3!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 5");
        query.exec("UPDATE referee SET role1 = 5 WHERE name = '" + cmb_judge3->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 5");
        query.exec("UPDATE referee SET role2 = 5 WHERE name = '" + cmb_judge3->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();

//    qDebug()<<sql;
//    if(cbFlag->isChecked())
//        flag_set(p->fr->judge_flag3, cmb_judge3->itemText(index));
}

void Settings::judge_4(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_4!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 6");
        query.exec("UPDATE referee SET role1 = 6 WHERE name = '" + cmb_judge4->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 6");
        query.exec("UPDATE referee SET role2 = 6 WHERE name = '" + cmb_judge4->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();

//    if(cbFlag->isChecked())
//        flag_set(p->fr->judge_flag4, cmb_judge4->itemText(index));
}

void Settings::judge_5(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных judge_5!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 7");
        query.exec("UPDATE referee SET role1 = 7 WHERE name = '" + cmb_judge5->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 7");
        query.exec("UPDATE referee SET role2 = 7 WHERE name = '" + cmb_judge5->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();

//    if(cbFlag->isChecked())
//        flag_set(p->fr->judge_flag5, cmb_judge5->itemText(index));
}

void Settings::ts(int){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(static_cast<FormMain*>(pW)->currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных TS!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    if(RbOne->isChecked()){
        query.exec("UPDATE referee SET role1 = 0 WHERE role1 = 8");
        query.exec("UPDATE referee SET role1 = 8 WHERE name = '" + cmb_ts->currentText() + "'");
    }else{
        query.exec("UPDATE referee SET role2 = 0 WHERE role2 = 8");
        query.exec("UPDATE referee SET role2 = 8 WHERE name = '" + cmb_ts->currentText() + "'");
    }
    (static_cast<FormMain*>(pW))->updateReferee();
    setCombo();
}

void Settings::writeReferees(void){

}

bool Settings::updateBase(QString sql){

}

void Settings::separate(bool state){
    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    if(state)
        settings.setValue("sep", "0");
    else
        settings.setValue("sep", "1");

    settings.endGroup();
}

void Settings::team_referees(bool state){
    setCombo();

}

void Settings::rus_to_eng(bool state){
    FormMain* p = static_cast<FormMain*>(pW);
    if(state){                //если выбрали Российское отображение
        p->fr->lbl_flag->setVisible(false);
        p->fr->grid->addWidget(p->fr->name, 0, 0, 3, 18);
        p->fr->grid->addWidget(p->fr->region, 0, 18, 3, 18);
        p->fr2->lbl_flag_blue->setVisible(false);
        p->fr2->lbl_flag_red->setVisible(false);
        p->fr2->grid->addWidget(p->fr2->region_blue, 5, 0,  4, 34);
        p->fr2->grid->addWidget(p->fr2->region_red,  5, 35, 4, 34);
        if(cbFlag->isChecked()){
            p->fr->judge_flag1->setVisible(false);
            p->fr->judge_flag2->setVisible(false);
            p->fr->judge_flag3->setVisible(false);

            p->fr2->judge_flag1->setVisible(false);
            p->fr2->judge_flag2->setVisible(false);
            p->fr2->judge_flag3->setVisible(false);
            p->fr2->judge_flag4->setVisible(false);
            p->fr2->judge_flag5->setVisible(false);

            p->fr->judge_flag4->setVisible(false);
            p->fr->judge_flag5->setVisible(false);

        }
        cbFlag->setEnabled(false);
    }
    else{
        p->fr->grid->addWidget(p->fr->name, 0, 0, 3, 26);
        p->fr->grid->addWidget(p->fr->region, 0, 26, 3, 6);

        p->fr->lbl_flag->setVisible(true);
        cbFlag->setEnabled(true);

        p->fr2->lbl_flag_blue->setVisible(true);
        p->fr2->lbl_flag_red->setVisible(true);

        p->fr2->grid->addWidget(p->fr2->region_blue,   5, 18, 4, 8);
        p->fr2->grid->addWidget(p->fr2->region_red,    5, 43, 4, 7);

        if(cbFlag->isChecked()){
            p->fr->judge_flag1->setVisible(true);
            p->fr->judge_flag2->setVisible(true);
            p->fr->judge_flag3->setVisible(true);

            p->fr2->judge_flag1->setVisible(true);
            p->fr2->judge_flag2->setVisible(true);
            p->fr2->judge_flag3->setVisible(true);
            p->fr2->judge_flag4->setVisible(true);
            p->fr2->judge_flag5->setVisible(true);

            p->fr->judge_flag4->setVisible(true);
            p->fr->judge_flag5->setVisible(true);
        }
    }
}

void Settings::showFlag(int state){
    FormMain* p = static_cast<FormMain*>(pW);
    if(state){
        p->fr->judge_flag1->setVisible(true);
        flag_set(p->fr->judge_flag1, p->judge1);
        p->fr->judge_flag2->setVisible(true);
        flag_set(p->fr->judge_flag2, p->judge2);
        p->fr->judge_flag3->setVisible(true);
        flag_set(p->fr->judge_flag3, p->judge3);

        p->fr2->judge_flag1->setVisible(true);
        flag_set(p->fr2->judge_flag1, p->judge1);
        p->fr2->judge_flag2->setVisible(true);
        flag_set(p->fr2->judge_flag2, p->judge2);
        p->fr2->judge_flag3->setVisible(true);
        flag_set(p->fr2->judge_flag3, p->judge3);
        p->fr2->judge_flag4->setVisible(true);
        flag_set(p->fr2->judge_flag4, p->judge4);
        p->fr2->judge_flag5->setVisible(true);
        flag_set(p->fr2->judge_flag5, p->judge5);

            //if self.p.ret_num:
        p->fr->judge_flag4->setVisible(true);
        flag_set(p->fr->judge_flag4, p->judge4);
        p->fr->judge_flag5->setVisible(true);
        flag_set(p->fr->judge_flag5, p->judge5);
    }
    else{

        p->fr->judge_flag1->setVisible(false);
        p->fr->judge_flag2->setVisible(false);
        p->fr->judge_flag3->setVisible(false);
        //if self.p.ret_num:
        p->fr->judge_flag4->setVisible(false);
        p->fr->judge_flag5->setVisible(false);

        p->fr2->judge_flag1->setVisible(false);
        p->fr2->judge_flag2->setVisible(false);
        p->fr2->judge_flag3->setVisible(false);
        p->fr2->judge_flag4->setVisible(false);
        p->fr2->judge_flag5->setVisible(false);
    }
}

void Settings::ChangeConnect2(void){
    QString port;
    if(sender() == btnReg2) port = btnReg2->text();
    else if (sender() == btnReg3) port = btnReg3->text();
    else if (sender() == btnReg4) port = btnReg4->text();
    else if (sender() == btnReg5) port = btnReg5->text();
    if(static_cast<FormMain*>(pW)->ret_mat)
        emit setPort("41" + port + "0");
    else
        emit setPort("42" + port + "0");
    qDebug() << port;
}

void Settings::registrationReset(void){
    FormMain* p = static_cast<FormMain*>(pW);

    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    settings.setValue("pult2", "0");
    settings.setValue("pult3", "0");
    settings.setValue("pult4", "0");
    settings.setValue("pult5", "0");
    settings.endGroup();

    btnReg2->setStyleSheet("color: #000000");
    btnReg3->setStyleSheet("color: #000000");
    btnReg4->setStyleSheet("color: #000000");
    btnReg5->setStyleSheet("color: #000000");

    p->pult2->ID = "0";
    p->pult3->ID = "0";
    p->pult4->ID = "0";
    p->pult5->ID = "0";

}

void Settings::registrationSuccesfull(QString ID, QString IP, QString PORT){
    FormMain* p = static_cast<FormMain*>(pW);

    delete reg;

    btnReg->setEnabled(true);
    btnReset->setEnabled(true);
    leCode->setText("");
    leCode->setEnabled(false);

    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    if(PORT[2] == '2'){
        settings.setValue("pult2", ID);
        p->pult2->ID = ID;
        btnReg2->setStyleSheet("color: #ff0000");
    }
    if(PORT[2] == '3'){
        settings.setValue("pult3", ID);
        p->pult3->ID = ID;
        btnReg3->setStyleSheet("color: #ff0000");    
    }

    if(PORT[2] == '4'){     
        settings.setValue("pult4", ID);
        p->pult4->ID = ID;
        btnReg4->setStyleSheet("color: #ff0000");
    }
    if(PORT[2] == '5'){
        settings.setValue("pult5", ID);
        p->pult5->ID = ID;
        btnReg5->setStyleSheet("color: #ff0000");
    }
    settings.endGroup();

    btnReg2->setEnabled(false);
    btnReg3->setEnabled(false);
    btnReg4->setEnabled(false);
    btnReg5->setEnabled(false);

}

void Settings::registrationFailed(void){
    delete reg;
    qDebug() << "registrationFailed";

    btnReg->setEnabled(true);
    btnReset->setEnabled(true);
    leCode->setText("");
    leCode->setEnabled(false);
    btnReg2->setEnabled(false);
    btnReg3->setEnabled(false);
    btnReg4->setEnabled(false);
    btnReg5->setEnabled(false); 
}

void Settings::registrationStart(QString data){
    leCode->setEnabled(true);
    leCode->setText(data);
    btnReg2->setEnabled(true);
    btnReg3->setEnabled(true);
    btnReg4->setEnabled(true);
    btnReg5->setEnabled(true);
}

void Settings::flag_set(QLabel* obj, QString sJudge){
    QPixmap pFlag = QPixmap();
    if(sJudge.length() == 0){
        obj->clear();
        return;
    }
        if(sJudge != "-"){
            QList<QString> stt = sJudge.split(";");
            QString path = "flags/";
            path = path + stt[1] + ".png";
            //if(os.path.exists(path)){
            QDir dir;
            if(dir.exists(path)){
                pFlag.load(path);
                obj->setPixmap(pFlag.scaled(QSize(obj->size().width()*0.9, obj->size().height()*0.9)));
                obj->setAlignment(Qt::AlignCenter);
            }else
                obj->clear();
        }else
            obj->clear();
}

void Settings::closeEvent(QCloseEvent* e){

}

bool Settings::getRbEng(void){
    bool state = rbEng->isChecked();
    return state;
}
