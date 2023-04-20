#include "FormMain.h"

#include <QDebug>
#include <QtSql>
//#include <QMessageBox>
#include <QIODevice>
#include <QLineEdit>
#include "choice_one_athlete.h"
#include "choice_two_athletes.h"
#include "choice.h"

#include "xlxs.h"

void Btn_next_clicked(FormMain*);
void fix_result(FormMain*);
int dialog(FormMain*);

QList<QString> f_sort(QList<QString> l){
    QList<QString> l_work;              //рабочий массив
    QList<int> l_int;                   //массив целых значений

    QString plus = NULL;                //строка, включающаая в себя "+"

    QRegExp rxDigit("\\d+");
    QRegExp rxPlus("\\+");

    int len = l.length();

    for(int i=0;i<len;i++){
        if (rxPlus.indexIn(l[i]) >= 0) plus = l[i];
        else {
            l_work.append(l[i]);
            if (rxDigit.indexIn(l[i]) >= 0) {
                l_int.append(rxDigit.cap().toInt());
            }
        }
    }

    len = l_work.length();

    int iTEMP;
    QString sTEMP;

    for(int i=0;i<(len-1);i++){
        for(int j=0;j<(len-1-i);j++){
            if (l_int[j] > l_int[j + 1]) {
                // меняем элементы местами
                iTEMP = l_int[j];
                l_int[j] = l_int[j + 1];
                l_int[j + 1] = iTEMP;
                sTEMP = l_work[j];
                l_work[j] = l_work[j + 1];
                l_work[j + 1] = sTEMP;
            }
        }
    }

    if(plus != NULL)
        l_work.append(plus);

    return l_work;
}

QString mask_to_string(int key_mask){
    int mask = 1;
    float rate = 10.0;
    for(int i=0;i<17;i++){
        if(i < 5){
            if((key_mask & mask) > 0)
                rate -= 0.5;
        }else if(i < 10){
            if((key_mask & mask) > 0)
                rate -= 1.0;
        }else if(i < 15){
            if((key_mask & mask) > 0)
                rate -= 2.0;
        }else if(i ==15){
            if((key_mask & mask) > 0)
                rate += 1.0;
        }else if(i == 16){
            if((key_mask & mask) > 0)
                rate = 0.0;
        }
        mask = mask << 1;
    }
    if(rate < 0.0)
        rate = 0.0;
    return QString::number(rate);
}

QString mask_to_list(int key_mask){
    QString l("[");
    bool first_error = true;
    int mask = 1;
    for(int i=0;i<17;i++){
        if((key_mask & mask) > 0){
            if(i == 15){
                if(first_error){
                    l.append("+1");
                    first_error = false;
                }else{
                    l.append(", +1");
                }
            }else if(i == 16){
                if(first_error){
                    l.append("0");
                    first_error = false;
                }else{
                    l.append(", 0");
                }
            }else{
                if(first_error){
                    l.append(QString::number(i + 1));
                    first_error = false;
                }else{
                    l.append(", " + QString::number(i + 1));
                }
            }
        }
        mask = mask << 1;
    }
    return l + "]";
}

TabFilter::TabFilter(QObject* parent) : QObject(parent){

}

bool TabFilter::eventFilter(QObject* po, QEvent* pe){
    if(pe->type() == QEvent::KeyPress){
        if(static_cast<QKeyEvent*>(pe)->key() == Qt::Key_Tab)
            emit focus();
        return QObject::eventFilter(po, pe);
    }
    return false;
}


FormMain::FormMain(QWidget *parent) : QWidget(parent)
{
    setupUi(this);



    flag_mode = 0;
    current_mode = -1;
    id_sportsmen = -1;
    id_red = -1;
    id_blue = -1;
    id_round = -1;
    id_rates = -1;
    id_rates_red = -1;
    id_rates_blue = -1;
    current_color = "black";
    first_color = 1;
    flag_shoise = 0;
    task = 0;
    name_competition = "";
    date = "";
    id_correct = 0;
    enable_correct = false;
    timer_id = 0;

    lref1 << "" << "" << "" << "" << "";
    lref2 << "" << "" << "" << "" << "";
    lref3 << "" << "" << "" << "" << "";
    lref4 << "" << "" << "" << "" << "";
    lref5 << "" << "" << "" << "" << "";

    lref1_red << "" << "" << "" << "" << "";
    lref2_red << "" << "" << "" << "" << "";
    lref3_red << "" << "" << "" << "" << "";
    lref4_red << "" << "" << "" << "" << "";
    lref5_red << "" << "" << "" << "" << "";

    lref1_red_blue << "" << "" << "" << "" << "";
    lref2_red_blue << "" << "" << "" << "" << "";
    lref3_red_blue << "" << "" << "" << "" << "";
    lref4_red_blue << "" << "" << "" << "" << "";
    lref5_red_blue << "" << "" << "" << "" << "";

    lref1_blue << "" << "" << "" << "" << "";
    lref2_blue << "" << "" << "" << "" << "";
    lref3_blue << "" << "" << "" << "" << "";
    lref4_blue << "" << "" << "" << "" << "";
    lref5_blue << "" << "" << "" << "" << "";

    main_refery = "-";
    main_secretary = "-";
    judge1 = "-";
    judge2 = "-";
    judge3 = "-";
    judge4 = "-";
    judge5 = "-";
    ts = "-";

    LBL_SUM = lbl_sum;
    LBL_TOTAL = lbl_total;

    LBL2 = lbl2;
    LBL3 = lbl3;
    LBL4 = lbl4;
    LBL5 = lbl5;
    Lbl_total_red = lbl_total_red;
    Lbl_total_blue = lbl_total_blue;
    Lbl_name = lbl_name;
    Lbl_name_red = lbl_name_red;
    Lbl_name_red->setStyleSheet("QLabel {color : red;}");
    Lbl_name_blue = lbl_name_blue;
    Lbl_name_blue->setStyleSheet("QLabel {color : blue;}");

    QList<QString> qvStr;

    Btn_next = btn_next;
    GroupBox_2 = groupBox_2;
    GroupBox = groupBox;
    Lbl_sum = lbl_sum;
    Lbl_q = lbl_q;
    Btn_enter = btn_enter;
    RbRed = rbRed;
    RbBlue = rbBlue;
    Cmb_round = cmb_round;
    CmbAge = cmbAge;
    CmbWeight = cmbWeight;
    Btn_new = btn_new;

    BtnChoice = btnChoice;
    BtnChoice->setVisible(false);

    Led1 = led1;
    Led2 = led2;
    Led3 = led3;
    Led4 = led4;
    Led5 = led5;

    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("baza.db");

    if(!QFileInfo ("baza.db").exists()){
        qDebug() << "Файл не существует" << endl;

        if (!m_db.open()) qDebug() << "Error: connection with database failed";
        else
        {
            qDebug() << "Database: connection ok";
            QSqlQuery query;

            QString str =   "CREATE TABLE sportsmens "
                            "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, "
                            "region TEXT, age TEXT, weight TEXT, "
                            "UNIQUE (name, weight, age))";

//            QString str =   "CREATE TABLE sportsmens "
//                            "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE COLLATE RTRIM, "
//                            "region TEXT, age TEXT, weight TEXT";

            if(!query.exec(str)) qDebug() << "UnaЫe to create а tаЫе sportsmens";

            str =           "CREATE TABLE errors_and_rates "
                            "(id INTEGER PRIMARY KEY AUTOINCREMENT, id_sportsmen INTEGER, id_round INTEGER, "
                            "errors1_1  TEXT, errors2_1 TEXT, errors3_1 TEXT, errors4_1 TEXT, errors5_1 TEXT, "
                            "errors1_2  TEXT, errors2_2 TEXT, errors3_2 TEXT, errors4_2 TEXT, errors5_2 TEXT, "
                            "errors1_3  TEXT, errors2_3 TEXT, errors3_3 TEXT, errors4_3 TEXT, errors5_3 TEXT, "
                            "errors1_4  TEXT, errors2_4 TEXT, errors3_4 TEXT, errors4_4 TEXT, errors5_4 TEXT, "
                            "errors1_5  TEXT, errors2_5 TEXT, errors3_5 TEXT, errors4_5 TEXT, errors5_5 TEXT, "
                            "rate1_1  TEXT, rate2_1 TEXT, rate3_1  TEXT, rate4_1 TEXT, rate5_1  TEXT, sum1 TEXT, "
                            "rate1_2  TEXT, rate2_2 TEXT, rate3_2  TEXT, rate4_2 TEXT, rate5_2  TEXT, sum2 TEXT, "
                            "rate1_3  TEXT, rate2_3 TEXT, rate3_3  TEXT, rate4_3 TEXT, rate5_3  TEXT, sum3 TEXT, "
                            "rate1_4  TEXT, rate2_4 TEXT, rate3_4  TEXT, rate4_4 TEXT, rate5_4  TEXT, sum4 TEXT, "
                            "rate1_5  TEXT, rate2_5 TEXT, rate3_5  TEXT, rate4_5 TEXT, rate5_5  TEXT, sum5 TEXT, "
                            "total TEXT, "
                            "UNIQUE (id_sportsmen, id_round))";

            if(!query.exec(str)) qDebug() << "UnaЫe to create а tаЫе errors_and_rates";

            str =           "CREATE TABLE rounds "
                            "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "round INTEGER, age  TEXT, weight TEXT, "
                            "mode INTEGER DEFAULT -1, "
                            "UNIQUE (round, age, weight))";

            if(!query.exec(str)) qDebug() << "UnaЫe to create а tаЫе rounds";


        }
    }
    else qDebug().noquote() << QString("Файл существует") << endl;

    if (!m_db.open()) qDebug() << "Error: connection with database failed";
    else {
        qDebug() << "Database: connection ok";
        QSqlQuery query;

        readSportsmens(&qvStr);     //чтение списка спортсменов из excel-файла
        int count = 0;
        QList<QString> qS;
        foreach(QString s, qvStr){
            qS.append(s);
            if(count++ == 3){
                QString str = "INSERT INTO sportsmens (name, region, age, weight) VALUES ('%1', '%2', '%3', '%4');";
                QString s;
                s =  str.arg(qS[0])
                        .arg(qS[1])
                        .arg(qS[2])
                        .arg(qS[3]);

                if(query.exec(s)) qDebug() << "Filled sportsmen's table" << s;
                else qDebug() << "error filled sportsmen's table" << s;
                count = 0;
                qS.clear();
            }
        }

        QString sql_age =           "SELECT DISTINCT (age) FROM sportsmens";
        QString sql_weight =        "SELECT DISTINCT weight FROM sportsmens WHERE age LIKE ('%1')";
        QString sql_round =         "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
        QString sql_round_insert =  "INSERT INTO rounds (round, age, weight) VALUES ('%1', '%2', '%3')";
        query.exec(sql_age);

        while(query.next()){
            qDebug() << endl;
            QString s = sql_weight.arg(query.value(0).toString());
            qDebug() << "value 3";
            QSqlQuery query_weight;
            query_weight.exec(s);
            while(query_weight.next()){
                QSqlQuery query_round;
                QString s = sql_round.arg(query.value(0).toString())
                                     .arg(query_weight.value(0).toString());
                qDebug() << "value 4";
                query_round.exec(s);
                int i = 0;
                while(query_round.next()){
                    qDebug() << query_round.value(0).toString();
                    i++;
                }
                if(i == 0){
                    QSqlQuery query_sql_round_insert;
                    QString s = sql_round_insert.arg("1")
                                                .arg(query.value(0).toString())
                                                .arg(query_weight.value(0).toString());
                    qDebug() << "value 5";
                    if(query_sql_round_insert.exec(s))
                        qDebug() << "insert success";
                    else
                        qDebug() << "didn't insert";
                }
            }
        }
        //////////////////////////////////////////////////////////
        /*           добавление элементов в комбобоксы          */
        /*               "возраст", "вес" и "круг"              */
        //////////////////////////////////////////////////////////
        query.exec("SELECT DISTINCT (age) FROM sportsmens");
        QList<QString> l_age;
        while(query.next()){
            l_age.append(query.value(0).toString());
            qDebug() << "value 6";
        }
        //std::sort(l_age.begin(), l_age.end());
        l_age = f_sort(l_age);
        foreach(QString sEach, l_age){
            cmbAge->addItem(sEach);
        }
        QString sql = "SELECT DISTINCT weight FROM sportsmens WHERE age = '%1'";
        sql = sql.arg(l_age[0]);
        query.exec(sql);
        QList<QString> l_weight;
        while(query.next()){
            l_weight.append(query.value(0).toString());
            qDebug() << "value 7";
        }

        //qDebug() << l_weight;
        //std::sort(l_weight.begin(), l_weight.end());
        l_weight = f_sort(l_weight);
        //qDebug() << l_weight;
        foreach(QString sEach, l_weight){
            cmbWeight->addItem(sEach);
        }
        sql_round = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
        sql_round =sql_round.arg(l_age[0])
                            .arg(l_weight[0]);
        query.exec(sql_round);
        QList<QString> l_round;
        while(query.next()){
            l_round.append(query.value(0).toString());
            qDebug() << "value 8";
        }
        //std::sort(l_round.begin(), l_round.end());
        foreach(QString sEach, l_round){
            cmb_round->addItem("круг " + sEach);
        }
        //qDebug() << "l_round.count() = " << l_round.count();
        cmb_round->setCurrentIndex(l_round.count() - 1);
    }

    keyVariant = true;      //вариант расладки клавиатуры
    rate = 10.0;            //оцена судьи за компьютером
    /*---состояние кнопок---*/
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
    /*-------Выбор ковра-------*/

    QDialog* kover = new QDialog();
    QFont font;
    font.setPointSize(14);
    font.setBold(true);

    kover->setWindowFlags(Qt::SplashScreen);
    QPushButton* btnMatA = new QPushButton("&Mat A");
    btnMatA->setFont(font);
    QPushButton* btnMatB = new QPushButton("&Mat B");
    btnMatB->setFont(font);
    QLabel* lbl = new QLabel("Выберите ковер");

    lbl->setFont(font);
    lbl->setAlignment(Qt::AlignHCenter);
    QHBoxLayout* hbox = new QHBoxLayout();
    QVBoxLayout* vbox = new QVBoxLayout();

    hbox->addWidget(btnMatA);
    hbox->addWidget(btnMatB);
    vbox->addWidget(lbl);
    vbox->addLayout(hbox);
    kover->setLayout(vbox);

    connect(btnMatA, SIGNAL(clicked()), kover, SLOT(accept()));
    connect(btnMatB, SIGNAL(clicked()), kover, SLOT(reject()));

    ret_mat = kover->exec();               //ковер A или B
    if (ret_mat == QDialog::Accepted){
        port2 = 4120;
        port3 = 4130;
        port4 = 4140;
        port5 = 4150;
        lblMat->setText("A");
    }else{
        port2 = 4220;
        port3 = 4230;
        port4 = 4240;
        port5 = 4250;
        lblMat->setText("B");
    }

    //////////////////////////////////////////////////////////

    //self.reg = sock_registration_udp.thread(self)
    //if self.ret_mat:
    //    self.reg.mat = b"matA"
    //else:
    //    self.reg.mat = b"matB"
    //self.reg.start()
    //////////////////////////////////////////////////////////

    //флаг подключения пульта: 0->2-отключен, 2->0-включен
    pult2_connect = 0;
    pult3_connect = 0;
    pult4_connect = 0;
    pult5_connect = 0;

    flag = QPixmap();
    flag_red = QPixmap();
    flag_blue = QPixmap();

    //флаг выполняемого приема: 0-прием не выполняется, 1...5-выполняется прием 1...5
    flag_priem = 0;

    num_round = 1;          //текущий круг соревнований
    count_round = 1;        //количество кругов
    total_sum = 0.0;
    total_sum_red = 0.0;
    total_sum_blue = 0.0;

    main_refery = "-";
    main_secretary = "-";
    judge1 = "-";
    judge2 = "-";
    judge3 = "-";
    judge4 = "-";
    judge5 = "-";
    ts = "-";

    file.setFileName("settings.txt");
    if(!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("-\n");
        file.write("-\n");
        file.write("01.01.2021\n");
        file.write("1\n");
        file.write("0");
        file.close();
    }

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        sity               = stream.readLine();
        name_competition   = stream.readLine();
        date               = stream.readLine();
        team_referees      = stream.readLine();
        RateShow           = stream.readLine();
        file.close();
    }

    qDebug() << sity << name_competition << date << team_referees << RateShow;

    file.setFileName("round_referees.txt");
    if(!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.close();
    }

    file.setFileName("round_referees2.txt");
    if(!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.write("0\n");
        file.close();
    }
    file.setFileName("main_referees.txt");
    if(!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("0\n");
        file.write("0\n");
        file.close();
    }

    if(ret_mat)
        file.setFileName("connectingA.txt");
    else
        file.setFileName("connectingB.txt");

    if(!file.exists()){
     file.open(QIODevice::WriteOnly | QIODevice::Text);
     file.write("0\n");
     file.write("0\n");
     file.write("0\n");
     file.write("0");
     file.close();
     }
    if(ret_mat)
        file.setFileName("connectingA.txt");
    else
        file.setFileName("connectingB.txt");

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        Conn2 = stream.readLine();
        Conn3 = stream.readLine();
        Conn4 = stream.readLine();
        Conn5 = stream.readLine();
        file.close();
    }

    sett = new Settings(sity, name_competition, date, Conn2, Conn3, Conn4, Conn5, this);

    err = new Errors(this);

    fr = new FirstRound(this);     //окно общего первого тура
    fr2 = new FirstRound2(this);   //окно попарного первого тура

    if(RateShow == "0")
        sett->RbnSeparate->setChecked(true);
    else
        sett->RbnTogether->setChecked(true);

    /*
            if self.team_referees == '1':
                self.sett.rbnOne.setChecked(True)
            else:
                self.sett.rbnTwo.setChecked(True)
    */

    sett->RbEng->setChecked(true);

    //QFont font;
    font.setPixelSize(30);

    lbl_total->setText("");

    connect(btn_next,       SIGNAL(clicked()), this, SLOT(btn_next_clicked()));
    connect(btn_enter,      SIGNAL(clicked()), this, SLOT(btn_enter_clicked()));

    connect(btn_enterName,  SIGNAL(clicked()), this, SLOT(btn_enterName_clicked()));
    connect(btn_0,          SIGNAL(clicked()), this, SLOT(btn_0_clicked()));
    connect(btn_1,          SIGNAL(clicked()), this, SLOT(btn_1_clicked()));
    connect(btn_2,          SIGNAL(clicked()), this, SLOT(btn_2_clicked()));
    connect(btn_3,          SIGNAL(clicked()), this, SLOT(btn_3_clicked()));
    connect(btn_4,          SIGNAL(clicked()), this, SLOT(btn_4_clicked()));
    connect(btn_6,          SIGNAL(clicked()), this, SLOT(btn_6_clicked()));
    connect(btn_7,          SIGNAL(clicked()), this, SLOT(btn_7_clicked()));
    connect(btn_8,          SIGNAL(clicked()), this, SLOT(btn_8_clicked()));
    connect(btn_9,          SIGNAL(clicked()), this, SLOT(btn_9_clicked()));
    connect(btn_10,         SIGNAL(clicked()), this, SLOT(btn_10_clicked()));
    connect(btn_5,          SIGNAL(clicked()), this, SLOT(btn_5_clicked()));
    connect(btn_11,         SIGNAL(clicked()), this, SLOT(btn_11_clicked()));
    connect(btn_look,       SIGNAL(clicked()), this, SLOT(btn_look_clicked()));     //протокол

    connect(btn_settings,   SIGNAL(clicked()), this, SLOT(btn_settings_clicked()));
    connect(btnErrors,      SIGNAL(clicked()), this, SLOT(btn_errors_clicked()));

    connect(led1, SIGNAL(textChanged(QString)), this, SLOT(set_rate1(QString)));
    connect(led2, SIGNAL(textChanged(QString)), this, SLOT(set_rate2(QString)));
    connect(led3, SIGNAL(textChanged(QString)), this, SLOT(set_rate3(QString)));
    connect(led4, SIGNAL(textChanged(QString)), this, SLOT(set_rate4(QString)));
    connect(led5, SIGNAL(textChanged(QString)), this, SLOT(set_rate5(QString)));

    connect(btnErr1,    SIGNAL(clicked()),      this, SLOT(btn_Err1()));
    connect(btnErr2,    SIGNAL(clicked()),      this, SLOT(btn_Err2()));
    connect(btnErr3,    SIGNAL(clicked()),      this, SLOT(btn_Err3()));
    connect(btnErr4,    SIGNAL(clicked()),      this, SLOT(btn_Err4()));
    connect(btnErr5,    SIGNAL(clicked()),      this, SLOT(btn_Err5()));
    connect(btnErr6,    SIGNAL(clicked()),      this, SLOT(btn_Err6()));
    connect(btnErr7,    SIGNAL(clicked()),      this, SLOT(btn_Err7()));
    connect(btnErr8,    SIGNAL(clicked()),      this, SLOT(btn_Err8()));
    connect(btnErr9,    SIGNAL(clicked()),      this, SLOT(btn_Err9()));
    connect(btnErr10,   SIGNAL(clicked()),      this, SLOT(btn_Err10()));
    connect(btnErr11,   SIGNAL(clicked()),      this, SLOT(btn_Err11()));
    connect(btnErr12,   SIGNAL(clicked()),      this, SLOT(btn_Err12()));
    connect(btnErr13,   SIGNAL(clicked()),      this, SLOT(btn_Err13()));
    connect(btnErr14,   SIGNAL(clicked()),      this, SLOT(btn_Err14()));
    connect(btnErr15,   SIGNAL(clicked()),      this, SLOT(btn_Err15()));
    connect(btnPlusOne, SIGNAL(clicked()),      this, SLOT(btn_PlusOne()));
    connect(btnZero,    SIGNAL(clicked()),      this, SLOT(btn_Zero()));
    connect(btnEnter,   SIGNAL(clicked()),      this, SLOT(btn_Enter()));

    pult2 = new Udp(port2, this);
    pult3 = new Udp(port3, this);
    pult4 = new Udp(port4, this);
    pult5 = new Udp(port5, this);

    qDebug() << Conn2 << Conn3 << Conn4 << Conn5;

    pult2->ID = Conn2;
    pult3->ID = Conn3;
    pult4->ID = Conn4;
    pult5->ID = Conn5;
    /*
    if(!(Conn2 == "0\n")){
        pult2.ID = self.Conn2[:-1];  //.split(';')[0]
        //pult2.IP = self.Conn2.split(';')[1][:-1]
     }
    */


    //tcp_reg = tcp.registration(self)
    //tcp_reg.start()

    connect(pult2, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult2, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult3, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult3, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult4, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult4, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult5, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult5, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(sett->LED_CITY,          SIGNAL(editingFinished()), this, SLOT(editSity()));
    connect(sett->NAME_COMPETITION,  SIGNAL(editingFinished()), this, SLOT(editSity()));
    connect(sett->DATEEDIT,          SIGNAL(editingFinished()), this, SLOT(editSity()));

    connect(btn_new, SIGNAL(clicked()), this, SLOT(new_round()));

    //------переключение клавиатуры (отслеживание нажатия клавиши 'TAB')------//
    TabFilter* tf1 = new TabFilter(led1);
    TabFilter* tf5 = new TabFilter(led5);
    led1->installEventFilter(tf1);
    led5->installEventFilter(tf5);

    connect(tf1, SIGNAL(focus()), this, SLOT(focusSet()));
    connect(tf5, SIGNAL(focus()), this, SLOT(focusReset()));

    style = btnErr1->styleSheet();

    connect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
    connect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));

    connect(rbFirst, SIGNAL(toggled(bool)), this, SLOT(switch_variant(bool)));

    cmb_round->emit currentIndexChanged(0);

    connect(btnChoice, SIGNAL(clicked()), this, SLOT(btnChoice_clicked()));

}

//////////////////////////////////
// функция нахождения id раунда //
//      и текущего режима       //
//////////////////////////////////

void FormMain::find_id_round(void){
    QString a = cmbAge->currentText();                      // возраст
    QString w = cmbWeight->currentText();                   // вес
    qDebug() << cmb_round->currentText();
    int r = cmb_round->currentText().remove(0, 5).toInt();  // номер круга
    QString sql = "SELECT id FROM rounds WHERE age = '%1' and weight = '%2' and round = '%3'";
    sql = sql.arg(a).arg(w).arg(r);
    QSqlQuery query;
    query.exec(sql);
    qDebug() << "find_id_round 1";
    query.next();
    id_round =  query.value(0).toInt();
    qDebug() << "value 9";
    sql = "SELECT mode FROM rounds WHERE id = '%1'";
    sql = sql.arg(id_round);
    query.exec(sql);
    qDebug() << "find_id_round 2";
    query.next();
    current_mode = query.value(0).toInt();
    qDebug() << "value 10";
    if(current_mode != -1){
        if(current_mode == 1){
            flag_mode = 1;
            flag_shoise = 0;
            switch_variant(false);          // режим попарный
            flag_shoise = 1;
            rbPairwise->setChecked(true);
        }else{
            flag_mode = 0;
            flag_shoise = 0;
            switch_variant(true)   ;        // режим общий
            flag_shoise = 1;
            rbFirst->setChecked(true);
        }
        groupBox->setEnabled(false);
    }else{
        flag_mode = 0;
        flag_shoise = 0;
        switch_variant(true);               // режим общий
        flag_shoise = 1;
        rbFirst->setChecked(true);
        groupBox->setEnabled(true);
    }
}

///////////////////////////////
//   выбор нового возраста   //
///////////////////////////////
void FormMain::age(void){
    QString age = cmbAge->currentText();
    flag_shoise = 1;
    cmbWeight->clear();
    QString sql = "SELECT DISTINCT weight FROM sportsmens WHERE age = '%1'";
    sql = sql.arg(age);
    QSqlQuery query;
    query.exec(sql);
    qDebug() << "age 1";
    QList<QString> l;
    while(query.next()){
        l.append(query.value(0).toString());
        qDebug() << "value 11";
    }
    //std::sort(l.begin(), l.end());
    l = f_sort(l);
    foreach(QString sEach, l){
        cmbWeight->addItem(sEach);
    }

    sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
    sql = sql.arg(age).arg(cmbWeight->currentText());
    l.clear();
    query.exec(sql);
    qDebug() << "age 2";
    while(query.next()){
        l.append(query.value(0).toString());
        qDebug() << "value 12";
    }
    //std::sort(l.begin(), l.end());
    l = f_sort(l);
    cmb_round->clear();
    foreach(QString sEach, l){
        cmb_round->addItem("круг " + sEach);
    }

    cmb_round->setCurrentIndex(l.length() - 1);
    find_id_round();
    flag_shoise = 0;
}

//////////////////////////////////////////
//          выбор нового веса           //
//////////////////////////////////////////
void FormMain::weight(void){
    QString weight = cmbWeight->currentText();
    if(flag_shoise == 0){
        flag_shoise = 1;
        QString sql_round = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
        sql_round = sql_round.arg(cmbAge->currentText()).arg(weight);
        QSqlQuery query;
        query.exec(sql_round);
        qDebug() << "weight 1";
        QList<QString> l;
        while(query.next()){
            l.append(query.value(0).toString());
            qDebug() << "value 13";
        }
        //std::sort(l.begin(), l.end());
        l = f_sort(l);
        cmb_round->clear();
        foreach(QString sEach, l){
            cmb_round->addItem("круг " + sEach);
        }
        cmb_round->setCurrentIndex(l.length() - 1);
        find_id_round();
        flag_shoise = 0;
    }
}

//////////////////////////////////////////
//          выбор нового круга          //
//////////////////////////////////////////
void FormMain::round_(){
    if(flag_shoise == 0){
        flag_shoise = 1;
        find_id_round();
        flag_shoise = 0;
    }
}

///////////////////////////////////////////////////////////
// Переключение вариантов табло: первый круг или попарно //
///////////////////////////////////////////////////////////
void FormMain::switch_variant(bool state){
    if(flag_shoise == 0){
        if(state){
            flag_mode = 0;
            fr2->hide();
            fr->show();
            lbl_total_red->setVisible(false);
            lbl_total_blue->setVisible(false);
            lbl_name_red->setVisible(false);
            lbl_name_blue->setVisible(false);
            groupBox_2->setEnabled(false);
            lbl_name->setVisible(true);
            lbl_total->setVisible(true);
            current_color = "black";
        }else{
            flag_mode = 1;
            fr->hide();
            fr2->show();
            lbl_total_red->setVisible(true);
            lbl_total_blue->setVisible(true);
            lbl_name_red->setVisible(true);
            lbl_name_blue->setVisible(true);
            groupBox_2->setEnabled(true);
            lbl_name->setVisible(false);
            lbl_total->setVisible(false);
            current_color = "red";
            first_color = 1;
            rbBlue->setChecked(true);
        }
    }
}



void FormMain::btn_Enter(void){
    if(flag_priem > 0){
        rate = 10.0;
        int mask = 0;
        if(stateErr1){
            rate -= 0.5;
            mask = mask | 1;
        }
        if(stateErr2){
            rate -= 0.5;
            mask = mask | 2;
        }
        if(stateErr3){
            rate -= 0.5;
            mask = mask | 4;
        }
        if(stateErr4){
            rate -= 0.5;
            mask = mask | 8;
        }
        if(stateErr5){
            rate -= 0.5;
            mask = mask | 16;
        }
        if(stateErr6){
            rate -= 1.0;
            mask = mask | 32;
        }
        if(stateErr7){
            rate -= 1.0;
            mask = mask | 64;
        }
        if(stateErr8){
            rate -= 1.0;
            mask = mask | 128;
        }
        if(stateErr9){
            rate -= 1.0;
            mask = mask | 256;
        }
        if(stateErr10){
            rate -= 1.0;
            mask = mask | 512;
        }
        if(stateErr11){
            rate -= 2.0;
            mask = mask | 1024;
        }
        if(stateErr12){
            rate -= 2.0;
            mask = mask | 2048;
        }
        if(stateErr13){
            rate -= 2.0;
            mask = mask | 4096;
        }
        if(stateErr14){
            rate -= 2.0;
            mask = mask | 8192;
        }
        if(stateErr15){
            rate -= 2.0;
            mask = mask | 16384;
        }
        if(statePlusOne){
            rate += 1.0;
            mask = mask | 32768;
        }
        if(stateZero){
            rate = 0.0;
            mask = mask | 65536;
        }
        if(rate < 0.0)
            rate = 0.0;

        rate_first(rate);
        if(flag_mode == 0){
            lref1[flag_priem - 1] = "";
            lref1[flag_priem - 1] = mask_to_list(mask);
        }else{
            lref1_red_blue[int((float)flag_priem / 2.0 + 0.5)- 1] = "";
            lref1_red_blue[int((float)flag_priem / 2.0 + 0.5) - 1] = mask_to_list(mask);
        }
    }
}

void FormMain::rate_first(float num){
    QList<QLineEdit*> widgetList = findChildren<QLineEdit*>();
    foreach(QLineEdit* le, widgetList){
        if(le->hasFocus()){
            if(num == 10)
                le->setText("10");
            else if(num == 11)
                le->setText("11");
            else{
                QString s;
                le->setText(s.setNum(num));
            }
        }
    }
}

void FormMain::btn_Err1(void){
    if(!stateErr1){
        stateErr1 = true;
        btnErr1->setStyleSheet("color: red");
    }else{
        stateErr1 = false;
        btnErr1->setStyleSheet(style);
    }
}

void FormMain::btn_Err2(void){
    if(!stateErr2){
        stateErr2 = true;
        btnErr2->setStyleSheet("color: red");
    }else{
        stateErr2 = false;
        btnErr2->setStyleSheet(style);
    }
}

void FormMain::btn_Err3(void){
    if(!stateErr3){
        stateErr3 = true;
        btnErr3->setStyleSheet("color: red");
    }else{
        stateErr3 = false;
        btnErr3->setStyleSheet(style);
    }
}

void FormMain::btn_Err4(void){
    if(!stateErr4){
        stateErr4 = true;
        btnErr4->setStyleSheet("color: red");
    }else{
        stateErr4 = false;
        btnErr4->setStyleSheet(style);
    }
}

void FormMain::btn_Err5(void){
    if(!stateErr5){
        stateErr5 = true;
        btnErr5->setStyleSheet("color: red");
    }else{
        stateErr5 = false;
        btnErr5->setStyleSheet(style);
    }
}

void FormMain::btn_Err6(void){
    if(!stateErr6){
        stateErr6 = true;
        btnErr6->setStyleSheet("color: red");
    }else{
        stateErr6 = false;
        btnErr6->setStyleSheet(style);
    }
}

void FormMain::btn_Err7(void){
    if(!stateErr7){
        stateErr7 = true;
        btnErr7->setStyleSheet("color: red");
    }else{
        stateErr7 = false;
        btnErr7->setStyleSheet(style);
    }
}

void FormMain::btn_Err8(void){
    if(!stateErr8){
        stateErr8 = true;
        btnErr8->setStyleSheet("color: red");
    }else{
        stateErr8 = false;
        btnErr8->setStyleSheet(style);
    }
}

void FormMain::btn_Err9(void){
    if(!stateErr9){
        stateErr9 = true;
        btnErr9->setStyleSheet("color: red");
    }else{
        stateErr9 = false;
        btnErr9->setStyleSheet(style);
    }
}

void FormMain::btn_Err10(void){
    if(!stateErr10){
        stateErr10 = true;
        btnErr10->setStyleSheet("color: red");
    }else{
        stateErr10 = false;
        btnErr10->setStyleSheet(style);
    }
}

void FormMain::btn_Err11(void){
    if(!stateErr11){
        stateErr11 = true;
        btnErr11->setStyleSheet("color: red");
    }else{
        stateErr11 = false;
        btnErr11->setStyleSheet(style);
    }
}

void FormMain::btn_Err12(void){
    if(!stateErr12){
        stateErr12 = true;
        btnErr12->setStyleSheet("color: red");
    }else{
        stateErr12 = false;
        btnErr12->setStyleSheet(style);
    }
}

void FormMain::btn_Err13(void){
    if(!stateErr13){
        stateErr13 = true;
        btnErr13->setStyleSheet("color: red");
    }else{
        stateErr13 = false;
        btnErr13->setStyleSheet(style);
    }
}

void FormMain::btn_Err14(void){
    if(!stateErr14){
        stateErr14 = true;
        btnErr14->setStyleSheet("color: red");
    }else{
        stateErr14 = false;
        btnErr14->setStyleSheet(style);
    }
}

void FormMain::btn_Err15(void){
    if(!stateErr15){
        stateErr15 = true;
        btnErr15->setStyleSheet("color: red");
    }else{
        stateErr15 = false;
        btnErr15->setStyleSheet(style);
    }
}

void FormMain::btn_PlusOne(void){
    if(!statePlusOne){
        statePlusOne = true;
        btnPlusOne->setStyleSheet("color: red");
    }else{
        statePlusOne = false;
        btnPlusOne->setStyleSheet(style);
    }
}

void FormMain::btn_Zero(void){
    if(!stateZero){
        stateZero = true;
        btnZero->setStyleSheet("color: red");
    }else{
        stateZero = false;
        btnZero->setStyleSheet(style);
    }
}

void FormMain::focusSet(void){
    keyVariant = false;
    btnErr1->setEnabled(false);
    btnErr2->setEnabled(false);
    btnErr3->setEnabled(false);
    btnErr4->setEnabled(false);
    btnErr5->setEnabled(false);
    btnErr6->setEnabled(false);
    btnErr7->setEnabled(false);
    btnErr8->setEnabled(false);
    btnErr9->setEnabled(false);
    btnErr10->setEnabled(false);
    btnErr11->setEnabled(false);
    btnErr12->setEnabled(false);
    btnErr13->setEnabled(false);
    btnErr14->setEnabled(false);
    btnErr15->setEnabled(false);
    btnEnter->setEnabled(false);
    btnPlusOne->setEnabled(false);
    btnZero->setEnabled(false);

    btn_1->setEnabled(true);
    btn_2->setEnabled(true);
    btn_3->setEnabled(true);
    btn_4->setEnabled(true);
    btn_5->setEnabled(true);
    btn_6->setEnabled(true);
    btn_7->setEnabled(true);
    btn_8->setEnabled(true);
    btn_9->setEnabled(true);
    btn_10->setEnabled(true);
    btn_11->setEnabled(true);
    btn_0->setEnabled(true);
}

void FormMain::focusReset(void){
    keyVariant = true;
    btnErr1->setEnabled(true);
    btnErr2->setEnabled(true);
    btnErr3->setEnabled(true);
    btnErr4->setEnabled(true);
    btnErr5->setEnabled(true);
    btnErr6->setEnabled(true);
    btnErr7->setEnabled(true);
    btnErr8->setEnabled(true);
    btnErr9->setEnabled(true);
    btnErr10->setEnabled(true);
    btnErr11->setEnabled(true);
    btnErr12->setEnabled(true);
    btnErr13->setEnabled(true);
    btnErr14->setEnabled(true);
    btnErr15->setEnabled(true);
    btnEnter->setEnabled(true);
    btnPlusOne->setEnabled(true);
    btnZero->setEnabled(true);

    btn_1->setEnabled(false);
    btn_2->setEnabled(false);
    btn_3->setEnabled(false);
    btn_4->setEnabled(false);
    btn_5->setEnabled(false);
    btn_6->setEnabled(false);
    btn_7->setEnabled(false);
    btn_8->setEnabled(false);
    btn_9->setEnabled(false);
    btn_10->setEnabled(false);
    btn_11->setEnabled(false);
    btn_0->setEnabled(false);
}

void FormMain::set_rate1(QString text){
    if(sett->RbnSeparate->isChecked()){
        if(text == "10.0" || text == "11.0")
            text = text.remove(text.length() - 2, 2);
        if(flag_mode == 0) fr->rate_1->setText(text);
        else fr2->rate_1->setText(text);
    }
}

void FormMain::set_rate2(QString text){
    if(sett->RbnSeparate->isChecked()){
        if(text == "10.0" || text == "11.0")
            text = text.remove(text.length() - 2, 2);
        if(flag_mode == 0) fr->rate_2->setText(text);
        else fr2->rate_2->setText(text);
    }
}

void FormMain::set_rate3(QString text){
    if(sett->RbnSeparate->isChecked()){
        if(text == "10.0" || text == "11.0")
            text = text.remove(text.length() - 2, 2);
        if(flag_mode == 0) fr->rate_3->setText(text);
        else fr2->rate_3->setText(text);
    }
}

void FormMain::set_rate4(QString text){
    if(sett->RbnSeparate->isChecked()){
        if(text == "10.0" || text == "11.0")
            text = text.remove(text.length() - 2, 2);
        if(flag_mode == 0) fr->rate_4->setText(text);
        else fr2->rate_4->setText(text);
    }
}

void FormMain::set_rate5(QString text){
    if(sett->RbnSeparate->isChecked()){
        if(text == "10.0" || text == "11.0")
            text = text.remove(text.length() - 2, 2);
        if(flag_mode == 0) fr->rate_5->setText(text);
        else fr2->rate_5->setText(text);
    }
}

void FormMain::btn_settings_clicked(void){
    sett->show();
}

void FormMain::btn_errors_clicked(void){
    err->show();
}

void FormMain::new_round(void){
    QSqlQuery query;
    QString sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
    sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
    query.exec(sql);
    QList<QString> l_round;
    while(query.next()){
        l_round.append(query.value(0).toString());
        qDebug() << "value 14";
    }
    //std::sort(l_round.begin(), l_round.end());
    int len_rounds = l_round.length();
    sql = "INSERT INTO rounds (round, age, weight) VALUES ('%1', '%2', '%3')";
    sql = sql.arg(len_rounds + 1).arg(cmbAge->currentText()).arg(cmbWeight->currentText());
    if(!query.exec(sql)) qDebug() << "unable insert rounds";
    else{
        cmb_round->addItem("круг " + QString::number(len_rounds + 1));
        cmb_round->setCurrentIndex(len_rounds);
    }
}

void FormMain::editSity(void){
    sity = sett->LED_CITY->text();
    name_competition = sett->NAME_COMPETITION->text();
    date = sett->DATEEDIT->text();
    QFile file("settings.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << sity << "\n"
           << name_competition << "\n"
           << date << "\n"
           << team_referees << "\n"
           << RateShow;
    file.close();
}

void FormMain::pult_off(int on_off, int port){
    if(port == port2){
        if(on_off){
            if(pult2_connect == 0){
                pult2_connect = 1;
                lbl2->setText("<font color=\"red\"><b>2</b></font>");
            }
        }
        else{
            if(pult2_connect == 1){
                pult2_connect = 0;
                lbl2->setText("<font color=\"black\">2</font>");
            }
        }
    }
    else if(port == port3){
        if(on_off){
            if(pult3_connect == 0){
                pult3_connect = 1;
                lbl3->setText("<font color=\"red\"><b>3</b></font>");
            }
        }
        else{
            if(pult3_connect == 1){
                pult3_connect = 0;
                lbl3->setText("<font color=\"black\">3</font>");
            }
        }
    }
    if(port == port4){
        if(on_off){
            if(pult4_connect == 0){
                pult4_connect = 1;
                lbl4->setText("<font color=\"red\"><b>4</b></font>");
            }
        }
        else{
            if(pult4_connect == 1){
                pult4_connect = 0;
                lbl4->setText("<font color=\"black\">4</font>");
            }
        }
    }
    if(port == port5){
        if(on_off){
            if(pult5_connect == 0){
                pult5_connect = 1;
                lbl5->setText("<font color=\"red\"><b>5</b></font>");
            }
        }
        else{
            if(pult5_connect == 1){
                pult5_connect = 0;
                lbl5->setText("<font color=\"black\">5</font>");
            }
        }
    }
}

////////////////////////////////////
//   Получение данных от пультов  //
////////////////////////////////////
void FormMain::pult(int rate, int port){
    if(task != 0){
        int value = rate;
        int priem = int((float)flag_priem / 2.0 + 0.5);
        if(rate != 255){
            QString srate = mask_to_string(rate);
            if((srate == "10.0") || (srate == "11.0"))
                srate = srate.remove(srate.length() - 2, 2);
            if(port == port2){
                led2->setText(srate);
                if(flag_mode == 0){
                    lref2[flag_priem - 1].clear();
                    lref2[flag_priem - 1].append(mask_to_list(value));
                }else{
                    lref2_red_blue[priem - 1].clear();
                    lref2_red_blue[priem - 1].append(mask_to_list(value));
                }
            }else if(port == port3){
                led3->setText(srate);
                if(flag_mode == 0){
                    lref3[flag_priem - 1].clear();
                    lref3[flag_priem - 1].append(mask_to_list(value));
                }else{
                    lref3_red_blue[priem - 1].clear();
                    lref3_red_blue[priem - 1].append(mask_to_list(value));
                }
            }else if(port == port4){
                led4->setText(srate);
                if(flag_mode == 0){
                    lref4[flag_priem - 1].clear();
                    lref4[flag_priem - 1].append(mask_to_list(value));
                }else{
                    lref4_red_blue[priem - 1].clear();
                    lref4_red_blue[priem - 1].append(mask_to_list(value));
                }
            }else if(port == port5){
                led5->setText(srate);
                if(flag_mode == 0){
                    lref5[flag_priem - 1].clear();
                    lref5[flag_priem - 1].append(mask_to_list(value));
                }else{
                    lref5_red_blue[priem - 1].clear();
                    lref5_red_blue[priem - 1].append(mask_to_list(value));
                }
            }
        }
    }
}

void FormMain::btn_next_clicked(void){
    Btn_next_clicked(this);
    timer_id = startTimer(2000);
}

void FormMain::timerEvent(QTimerEvent*){
    killTimer(timer_id);
    for(int count=2;count<6;count++){
        QLineEdit* led = findChild<QLineEdit*>("led" + QString::number(count));
        led->setText("");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// проверка на правильность и полноту введенных значений для варианта табло с 5 судьями //
//////////////////////////////////////////////////////////////////////////////////////////
int FormMain::min_max(void){
    float _min = 11.0;
    float _max = 0.0;
    int count_min = 1;
    int count_max = 1;
    float summa = 0.0;
    try{
        for(int count=1;count<6;count++){
            QLineEdit* led = findChild<QLineEdit*>("led" + QString::number(count));
            if(led->text().length() == 0)
                return 1;
            float val = led->text().toFloat();
            if(val < _min){
                _min = led->text().toFloat();
                count_min = count;
            }
            if(val >= _max){
                _max = led->text().toFloat();
                count_max = count;
            }
            summa += val;
        }
        QLineEdit* led = findChild<QLineEdit*>("led" + QString::number(count_min));
        summa -= led->text().toFloat();
        led->setStyleSheet("QLineEdit {background-color: blue;}");
        led = findChild<QLineEdit*>("led" + QString::number(count_max));
        summa -= led->text().toFloat();
        led->setStyleSheet("QLineEdit {background-color: red;}");

        lbl_sum->setText(QString::number(summa));

        QFile file("style_min_max.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = file.readAll();
        file.close();

        if(count_min == 1 || count_max == 1){
            if(flag_mode == 0) fr->rate_1->setStyleSheet(styleSheet);
            else fr2->rate_1->setStyleSheet(styleSheet);
        }
        if(count_min == 2 || count_max == 2){
            if(flag_mode == 0) fr->rate_2->setStyleSheet(styleSheet);
            else fr2->rate_2->setStyleSheet(styleSheet);
        }
        if(count_min == 3 || count_max == 3){
            if(flag_mode == 0) fr->rate_3->setStyleSheet(styleSheet);
            else fr2->rate_3->setStyleSheet(styleSheet);
        }
        if(count_min == 4 or count_max == 4){
            if(flag_mode == 0) fr->rate_4->setStyleSheet(styleSheet);
            else fr2->rate_4->setStyleSheet(styleSheet);
        }
        if(count_min == 5 || count_max == 5){
            if(flag_mode == 0) fr->rate_5->setStyleSheet(styleSheet);
            else fr2->rate_5->setStyleSheet(styleSheet);
        }
        if(flag_mode == 0) fr->rate_sum->setText(QString::number(summa));
        else fr2->rate_sum->setText(QString::number(summa));
        for(int count=1;count<6;count++){
            QLineEdit* led = findChild<QLineEdit*>("led" + QString::number(count));
            led->setEnabled(false);
        }
        return 0;
    }catch(...){
        return 1;
    }
}

void FormMain::clear_keys(void){
    btnPlusOne->setStyleSheet(style);
    btnZero->setStyleSheet(style);
    btnErr1->setStyleSheet(style);
    btnErr2->setStyleSheet(style);
    btnErr3->setStyleSheet(style);
    btnErr4->setStyleSheet(style);
    btnErr5->setStyleSheet(style);
    btnErr6->setStyleSheet(style);
    btnErr7->setStyleSheet(style);
    btnErr8->setStyleSheet(style);
    btnErr9->setStyleSheet(style);
    btnErr10->setStyleSheet(style);
    btnErr11->setStyleSheet(style);
    btnErr12->setStyleSheet(style);
    btnErr13->setStyleSheet(style);
    btnErr14->setStyleSheet(style);
    btnErr15->setStyleSheet(style);
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
    focusReset();
}

////////////////////////////////////////////////////////////////////
//               фиксация ошибок в текущем вопросе                //
////////////////////////////////////////////////////////////////////
void FormMain::btn_enter_clicked(void){
    if(min_max())    //проверка на правильность и полноту введенных значений
        return;
    clear_keys();
    btn_enter->setEnabled(false);
    task = 0;
    fix_result(this);
    pult2->task = 0;
    pult3->task = 0;
    pult4->task = 0;
    pult5->task = 0;
}

void FormMain::btn_enterName_clicked(){
    if(dialog(this) == 0)
        return;
    QString age = cmbAge->currentText();
    QString weight = cmbWeight->currentText();
    QString round_ = QString::number(cmb_round->currentIndex() + 1);
    // выбор спортсменов для списка по возрасту и весу, исключая ранее выступавших в этом кругу
    QString sql = "SELECT id FROM sportsmens WHERE age = '%1' and weight = '%2' "
                  "EXCEPT "
                  "SELECT id_sportsmen FROM errors_and_rates WHERE id_round = " + QString::number(id_round);
    sql = sql.arg(age).arg(weight);
    QSqlQuery query;
    QStringList sL;
    QString sIN;
    sIN = "";
    if(!query.exec(sql)){
        qDebug() << "error s rate";
    }else{
        while(query.next()){
            //sL.append(query.value(0).toString());
            if(sIN == "")
                sIN = sIN + query.value(0).toString();
            else
                sIN = sIN + ", " + query.value(0).toString();
        }
    }
    qDebug() << "value 1";
    sql = "SELECT name, region FROM sportsmens WHERE id IN (" + sIN + ")";
    QStringList data;
    if(!query.exec(sql))
        qDebug() << "error sel name, region";
    else{
        while(query.next()){
            data.append(query.value(0).toString() + ";" + query.value(1).toString());
            qDebug() << "value 2";
        }
    }


    if(flag_mode == 0){
        Choice_one_athlete* listNameOne = new Choice_one_athlete(data, this);    // окно выбора спортсменов общего круга
        total_sum = 0.0;
        QString s;
        s = s.setNum(total_sum);
        lbl_total->setText(s);
    }else{
        Choice_two_athletes* listNameTwo = new Choice_two_athletes(data, this);
        total_sum_red = 0.0;
        total_sum_blue = 0.0;
        QString s;
        s.setNum(total_sum_red);
        lbl_total_red->setText(s);
        s.setNum(total_sum_blue);
        lbl_total_blue->setText(s);
        fr2->lbl_priem->setText("TASK\n");
        fr2->lbl_priem->setStyleSheet("QLabel { background-color : black; color : white; }");
    }


    for(int count=1;count<6;count++){
        QLineEdit* led = findChild<QLineEdit*>("led" + QString::number(count));
        led->setStyleSheet("QLineEdit {background-color: #F0F0F0;}");
        led->setText("");
        led->setEnabled(false);
    }
    lbl_sum->setText("");
    lbl_q->setText("");
    flag_priem = 0;
}

void FormMain::button_num(QString num){
    QList<QLineEdit*> widgetList = findChildren<QLineEdit*>();
    foreach(QLineEdit* each, widgetList){
        if(each->hasFocus()){
            if(each->text().length() == 0)
                each->setText(num);
        }
    }
}

void FormMain::btn_0_clicked(){
    button_num("0.0");
}

void FormMain::btn_1_clicked(){
    button_num("1.0");
}

void FormMain::btn_2_clicked(){
    button_num("2.0");
}

void FormMain::btn_3_clicked(){
    button_num("3.0");
}

void FormMain::btn_4_clicked(){
    button_num("4.0");
}

void FormMain::btn_5_clicked(){
    QList<QLineEdit*> widgetList = findChildren<QLineEdit*>();
    foreach(QLineEdit* each, widgetList){
        if(each->hasFocus()){
            if(each->text().length() == 3)
                each->setText(each->text().remove(2, each->text().length() - 2) + "5");
            else if(each->text() == "10")
                each->setText("10.5");
            else if(each->text().length() == 0)
                each->setText("5.0");
        }
    }
}

void FormMain::btn_6_clicked(){
    button_num("6.0");
}

void FormMain::btn_7_clicked(){
    button_num("7.0");
}

void FormMain::btn_8_clicked(){
    button_num("8.0");
}

void FormMain::btn_9_clicked(){
    button_num("9.0");
}

void FormMain::btn_10_clicked(){
    button_num("10");
}

void FormMain::btn_11_clicked(){
    button_num("11");
}

void FormMain::keyPressEvent(QKeyEvent* e){
    if(e->key() == Qt::Key_Escape)
        QApplication::quit();
}

void FormMain::btn_look_clicked(){
    ShowReports* shR = new ShowReports(sity, name_competition, date, num_round, this);
    shR->show();
}

void FormMain::btnChoice_clicked(){
    choice* c = new choice();
    c->setWindowFlags(Qt::WindowStaysOnTopHint);
    c->show();

}

/*
bool FormMain::event(QEvent *event){
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            // здесь обрабатывается специальная табуляция
            qDebug() << "Tab";
            return true;
        }
     return QWidget::event(event);
    }
}
*/
/*
void FormMain::on_led1_selectionChanged()
{

}
*/
