#include "settings.h"
#include "xlxs.h"
#include "FormMain.h"

Settings::Settings(QString _city,
                   QString _name_competition,
                   QString _date,
                   QString _conn2,
                   QString _conn3,
                   QString _conn4,
                   QString _conn5,
                   QWidget *parent):QWidget(){

    setupUi(this);

    RbnSeparate = rbnSeparate;
    RbnTogether = rbnTogether;
    RbEng = rbEng;

    LED_CITY = led_city;
    NAME_COMPETITION = name_competition;
    DATEEDIT = dateEdit;

    date = _date;
    conn2 = _conn2;
    conn3 = _conn3;
    conn4 = _conn4;
    conn5 = _conn5;

    ip = "";
    code = "";
    id = "";
    regFail = true;
    //pW = static_cast<FormMain*>(parent);
    pW = parent;
    FormMain* p = static_cast<FormMain*>(pW);

    led_city->setText(_city);
    name_competition->setText(_name_competition);

    qDate.fromString(_date, "dd.MM.YYYY");
    dateEdit->setDate(qDate);
    QList<QString> l;
    readReferees(&l);
    //qDebug() << l;
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

    QFile file;

    file.setFileName("main_referees.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);

        int i = stream.readLine().toInt();
        cmb_main_judge->setCurrentIndex(i);
        p->main_refery = cmb_main_judge->itemText(i);

        i = stream.readLine().toInt();
        cmb_main_secretary->setCurrentIndex(i);
        p->main_secretary = cmb_main_secretary->itemText(i);

        file.close();
    }

    if(static_cast<FormMain*>(pW)->team_referees == "1"){
        file.setFileName("round_referees.txt");
        rbnOne->setChecked(true);
    }else{
        file.setFileName("round_referees2.txt");
        rbnTwo->setChecked(true);
    }

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);

        int i = stream.readLine().toInt();
        cmb_judge1->setCurrentIndex(i);
        p->judge1 = cmb_judge1->itemText(i);

        i = stream.readLine().toInt();
        cmb_judge2->setCurrentIndex(i);
        p->judge2 = cmb_judge2->itemText(i);

        i = stream.readLine().toInt();
        cmb_judge3->setCurrentIndex(i);
        p->judge3 = cmb_judge3->itemText(i);

        i = stream.readLine().toInt();
        cmb_judge4->setCurrentIndex(i);
        p->judge4 = cmb_judge4->itemText(i);

        i = stream.readLine().toInt();
        cmb_judge5->setCurrentIndex(i);
        p->judge5 = cmb_judge5->itemText(i);

        i = stream.readLine().toInt();
        cmb_ts->setCurrentIndex(i);
        p->ts = cmb_ts->itemText(i);

        file.close();
    }

    if(_conn2 != "0\n")
        btnReg2->setStyleSheet("color: #ff0000");
    else
        btnReg2->setStyleSheet("color: #000000");
    if(_conn3 != "0\n")
        btnReg3->setStyleSheet("color: #ff0000");
    else
        btnReg3->setStyleSheet("color: #000000");
    if(_conn4 != "0\n")
        btnReg4->setStyleSheet("color: #ff0000");
    else
        btnReg4->setStyleSheet("color: #000000");
    if(_conn5 != "0")
        btnReg5->setStyleSheet("color: #ff0000");
    else
        btnReg5->setStyleSheet("color: #000000");

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
    connect(rbRus, SIGNAL(toggled(bool)), SLOT(rus_to_eng(bool)));
    connect(cbFlag, SIGNAL(stateChanged(int)), SLOT(showFlag(int)));
    connect(btnReg2, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg3, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg4, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReg5, SIGNAL(clicked(bool)), SLOT(ChangeConnect2()));
    connect(btnReset, SIGNAL(clicked(bool)), SLOT(registrationReset()));

    //connect(p->reg, SIGNAL(registration(QVariant, QVariant, QVariant)),
    //                                        SLOT(registration()));
    //connect(p->reg, SIGNAL(registrationStart(QString)),
    //                                        SLOT(registrationStart()), Qt::QueuedConnection);
    //connect(p->reg, SIGNAL(registrationFailed()),
    //                                        SLOT(registrationFailed()), Qt::QueuedConnection);
    //connect(reg, SIGNAL(registrationSuccesfull(QString, QString, QString)), this,
    //                                        SLOT(registrationSuccesfull()));

    //connect(rbA, SIGNAL("toggled(bool)"), SLOT(choice_mat()));

    leCode->setText("1234");
}

void Settings::registration(void){
    FormMain* p = static_cast<FormMain*>(pW);
    //Registration* reg;
    //reg = sock_registration_udp.thread();
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

void Settings::judge_main(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->main_refery = cmb_main_judge->itemText(index);
    QFile file("main_referees.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << QString::number(cmb_main_judge->currentIndex()) << endl;
    stream << QString::number(cmb_main_secretary->currentIndex()) << endl;
    file.close();
}

void Settings::judge_secretary(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->main_secretary = cmb_main_secretary->itemText(index);
    QFile file("main_referees.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << QString::number(cmb_main_judge->currentIndex()) << endl;
    stream << QString::number(cmb_main_secretary->currentIndex()) << endl;
    file.close();
}

void Settings::judge_1(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->judge1 = cmb_judge1->itemText(index);
    writeReferees();
    if(cbFlag->isChecked())
        flag_set(p->fr->judge_flag1, cmb_judge1->itemText(index));
}

void Settings::judge_2(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->judge2 = cmb_judge2->itemText(index);
    writeReferees();
    if(cbFlag->isChecked())
        flag_set(p->fr->judge_flag2, cmb_judge2->itemText(index));
}

void Settings::judge_3(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->judge3 = cmb_judge3->itemText(index);
    writeReferees();
    if(cbFlag->isChecked())
        flag_set(p->fr->judge_flag3, cmb_judge3->itemText(index));
}

void Settings::judge_4(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->judge4 = cmb_judge4->itemText(index);
    writeReferees();
    if(cbFlag->isChecked())
        flag_set(p->fr->judge_flag4, cmb_judge4->itemText(index));
}

void Settings::judge_5(int index){
    FormMain* p = static_cast<FormMain*>(pW);
    p->judge5 = cmb_judge5->itemText(index);
    writeReferees();
    if(cbFlag->isChecked())
        flag_set(p->fr->judge_flag5, cmb_judge5->itemText(index));
}

void Settings::ts(int index){
    static_cast<FormMain*>(pW)->ts = cmb_ts->itemText(index);
    writeReferees();
}

void Settings::writeReferees(void){
    QFile file;
    if(rbnOne->isChecked())
        file.setFileName("round_referees.txt");
    else
        file.setFileName("round_referees2.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << QString::number(cmb_judge1->currentIndex()) << endl;
    stream << QString::number(cmb_judge2->currentIndex()) << endl;
    stream << QString::number(cmb_judge3->currentIndex()) << endl;
    stream << QString::number(cmb_judge4->currentIndex()) << endl;
    stream << QString::number(cmb_judge5->currentIndex()) << endl;
    stream << QString::number(cmb_ts->currentIndex()) << endl;
    file.close();
}

void Settings::separate(bool state){
    QFile file("settings.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << led_city->text() << endl;
    stream << name_competition->text() << endl;
    stream << dateEdit->text() << endl;
    if(rbnOne->isChecked())
        stream << "1" << endl;
    else
        stream << "2" << endl;
    if(state)
        stream << "0" << endl;
    else
        stream << "1" << endl;
    file.close();
}

void Settings::team_referees(bool state){
    FormMain* p = static_cast<FormMain*>(pW);
    QFile file("settings.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << led_city->text() << endl;
    stream << name_competition->text() << endl;
    stream << dateEdit->text() << endl;
    if(state)
        stream << "1" << endl;
    else
        stream << "2" << endl;
    if(rbnSeparate->isChecked())
        stream << "0";
    else
        stream << "1";
    file.close();
    if(state)
        file.setFileName("round_referees.txt");
    else
        file.setFileName("round_referees2.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream2(&file);

    int i = stream2.readLine().toInt();
    cmb_judge1->setCurrentIndex(i);
    p->judge1 = cmb_judge1->itemText(i);

    i = stream2.readLine().toInt();
    cmb_judge2->setCurrentIndex(i);
    p->judge2 = cmb_judge2->itemText(i);

    i = stream2.readLine().toInt();
    cmb_judge3->setCurrentIndex(i);
    p->judge3 = cmb_judge3->itemText(i);

    i = stream2.readLine().toInt();
    cmb_judge4->setCurrentIndex(i);
    p->judge4 = cmb_judge4->itemText(i);

    i = stream2.readLine().toInt();
    cmb_judge5->setCurrentIndex(i);
    p->judge5 = cmb_judge5->itemText(i);

    i = stream2.readLine().toInt();
    cmb_ts->setCurrentIndex(i);
    p->ts = cmb_ts->itemText(i);

    file.close();
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

    btnReg2->setStyleSheet("color: #000000");
    btnReg3->setStyleSheet("color: #000000");
    btnReg4->setStyleSheet("color: #000000");
    btnReg5->setStyleSheet("color: #000000");

    p->pult2->ID = "0";
    //p->pult2.IP = "-";
    p->pult3->ID = "0";
    //p->pult3.IP = "-";
    p->pult4->ID = "0";
    //p->pult4.IP = "-";
    p->pult5->ID = "0";
    //p->pult5.IP = "-";

    QFile file;
    if(static_cast<FormMain*>(pW)->ret_mat)
        file.setFileName("connectingA.txt");
    else
        file.setFileName("connectingB.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write("0\n0\n0\n0");
    file.close();
}

void Settings::registrationSuccesfull(QString ID, QString IP, QString PORT){
    FormMain* p = static_cast<FormMain*>(pW);

    delete reg;

    btnReg->setEnabled(true);
    btnReset->setEnabled(true);
    leCode->setText("");
    leCode->setEnabled(false);
    QFile file;
    if(p->ret_mat)
        file.setFileName("connectingA.txt");
    else
        file.setFileName("connectingB.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QList<QString> lines;
    while (!stream.atEnd())
        lines.append(stream.readLine() + '\n');
    lines[3].truncate(lines[3].length() - 1);
    file.close();
    if (PORT[2] == '2'){
        lines[0] = ID + '\n'; //';' + IP + '\n';
        //self.p.reg.conn = str(self.p.port2)
        p->pult2->ID = ID;
        //self.p.pult2.IP = IP
        btnReg2->setStyleSheet("color: #ff0000");
        /*
        if (self.p.ret_mat){
            self.p.tcp_reg.val = b'4120'
        else:
            self.p.tcp_reg.val = b'4220'
            #self.cbConn2.setEnabled(False)
        }*/
    }
    if( PORT[2] == '3'){
        lines[1] = ID + '\n'; //#';' + IP + '\n'
        //self.p.reg.conn = str(self.p.port3)

        p->pult3->ID = ID;
        //self.p.pult3.IP = IP
        btnReg3->setStyleSheet("color: #ff0000");
        /*
        if self.p.ret_mat:
            self.p.tcp_reg.val = b'4130'
        else:
            self.p.tcp_reg.val = b'4230'
            #self.cbConn3.setEnabled(False)
        */
    }
        //if self.p.ret_num:
    if(PORT[2] == '4'){
        lines[2] = ID + '\n'; //#';' + IP + '\n'
        //#self.p.reg.conn = str(self.p.port4)

        p->pult4->ID = ID;
        //#self.p.pult4.IP = IP
        btnReg4->setStyleSheet("color: #ff0000");
        /*'''
                if self.p.ret_mat:
                    self.p.tcp_reg.val = b'4140'
                else:
                    self.p.tcp_reg.val = b'4240'
                #self.cbConn4.setEnabled(False)
        '''*/
    }
    if(PORT[2] == '5'){
        lines[3] = ID ;//# + ';' + IP
        //#self.p.reg.conn = str(self.p.port5)

        p->pult5->ID = ID;
        //#self.p.pult5.IP = IP
        btnReg5->setStyleSheet("color: #ff0000");
        /*'''
                if self.p.ret_mat:
                    self.p.tcp_reg.val = b'4150'
                else:
                    self.p.tcp_reg.val = b'4250'
                #self.cbConn5.setEnabled(False)
        '''*/
    }
    if(p->ret_mat)
        file.setFileName("connectingA.txt");
    else
        file.setFileName("connectingB.txt");
        //#else:
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream2(&file);
    foreach(QString each, lines)
        stream2 << each;
    file.close();
    btnReg2->setEnabled(false);
    btnReg3->setEnabled(false);
    btnReg4->setEnabled(false);
    btnReg5->setEnabled(false);
    //delete reg;
}

void Settings::registrationFailed(void){
    delete reg;
    qDebug() << "registrationFailed";
        /*
        '''
        if not self.regFail:
            self.leCode.setText('')
            self.regFail = True
            self.ip = ''
            self.code = ''
            self.id = ''
            print('f')
        '''
        */
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
    /*
    QFile file;
    if ((static_cast<FormMain*>(pW)->team_referees) == "1")
        file.setFileName("round_referees.txt");
    else
        file.setFileName("round_referees2.txt");
    file.open((QIODevice::WriteOnly | QIODevice::Text));
    QTextStream stream(&file);
    //stream << QString::number(cmb_main_judge->currentIndex()) << endl;
    //stream << QString::number(cmb_main_secretary->currentIndex()) << endl;
    stream << QString::number(cmb_judge1->currentIndex()) << endl;
    stream << QString::number(cmb_judge2->currentIndex()) << endl;
    stream << QString::number(cmb_judge3->currentIndex()) << endl;
    stream << QString::number(cmb_judge4->currentIndex()) << endl;
    stream << QString::number(cmb_judge5->currentIndex()) << endl;
    stream << QString::number(cmb_ts->currentIndex()) << endl;
    file.close();
    */
}

bool Settings::getRbEng(void){
    bool state = rbEng->isChecked();
    return state;
}
