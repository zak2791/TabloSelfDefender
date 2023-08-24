#include "FormMain.h"

#include <QDebug>
#include <QtSql>
//#include <QMessageBox>
#include <QIODevice>
#include <QLineEdit>
#include "choice_one_athlete.h"
#include "choice_two_athletes.h"
#include "choice.h"
#include "addcompetition.h"
//#include "xlxs.h"
#include "ui_addOneSportsmen.h"
#include "ui_addcompetition.h"

#include "protocol.h"
#include "dlgsemifinal.h"

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


FormMain::FormMain(MainWin* mw, QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    mainwin = mw;
    connect(mainwin, SIGNAL(sigChoiceCompetitions(QString)), this, SLOT(choiceCompetitions(QString)));

    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    QString mat = settings.value("mat").toString();
    if(mat == ""){
        mat = "А";
        settings.setValue( "mat", mat);
    }
    if(settings.value("pult2").toString() == ""){
        settings.setValue( "pult2", "0");
        settings.setValue( "pult3", "0");
        settings.setValue( "pult4", "0");
        settings.setValue( "pult5", "0");
    }
    settings.endGroup();
//    settings.beginGroup("Ref");
//    judge1 = settings.value("ref1").toString();
//    judge2 = settings.value("ref2").toString();
//    judge3 = settings.value("ref3").toString();
//    judge4 = settings.value("ref4").toString();
//    judge5 = settings.value("ref5").toString();

//    settings.beginGroup("Ref");

    mainwin->choiceMat(mat);
    lblMat->setText(mat);
    if (mat == "А"){
        port2 = 4120;
        port3 = 4130;
        port4 = 4140;
        port5 = 4150;
        lblMat->setText("A");
        ret_mat = 1;
    }
    if (mat == "Б"){
        port2 = 4220;
        port3 = 4230;
        port4 = 4240;
        port5 = 4250;
        lblMat->setText("Б");
        ret_mat = 0;
    }

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
    BtnFinal = btnFin;
    BtnSemiFinal = btnSemiFin;
    btnEnterName = btn_enterName;
    Btn_change = btnChange;

    //BtnChoice = btnChoice;
    //BtnChoice->setVisible(false);

    Led1 = led1;
    Led2 = led2;
    Led3 = led3;
    Led4 = led4;
    Led5 = led5;



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

//       if(file.open(QIODevice::ReadOnly)){
//        QTextStream stream(&file);
//        sity               = stream.readLine();
//        name_competition   = stream.readLine();
//        date               = stream.readLine();
//        team_referees      = stream.readLine();
//        RateShow           = stream.readLine();
//        file.close();
//    }

    //qDebug() << sity << name_competition << date << team_referees << RateShow;

//    file.setFileName("round_referees.txt");
//    if(!file.exists()){
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.close();
//    }

//    file.setFileName("round_referees2.txt");
//    if(!file.exists()){
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.write("0\n");
//        file.close();
//    }
//    file.setFileName("main_referees.txt");
//    if(!file.exists()){
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        file.write("0\n");
//        file.write("0\n");
//        file.close();
//    }

//    if(mat == "А")
//        file.setFileName("connectingA.txt");
//    if(mat == "Б")
//        file.setFileName("connectingB.txt");


    settings.beginGroup("Settings");
    Conn2 = settings.value("pult2").toString();
    Conn3 = settings.value("pult3").toString();
    Conn4 = settings.value("pult4").toString();
    Conn5 = settings.value("pult5").toString();
    settings.endGroup();


    sett = new Settings(this);

    err = new Errors(this);

    fr = new FirstRound(this);     //окно общего первого тура
    fr2 = new FirstRound2(this);   //окно попарного первого тура

    if(RateShow == "0")
        sett->RbnSeparate->setChecked(true);
    else
        sett->RbnTogether->setChecked(true);

    //sett->RbEng->setChecked(true);

    lbl_total->setText("");

    //Protocol* prt = new Protocol(this);
    //QPushButton* BtnNewFinal = btnNewFinal;
    connect(btn_look,       SIGNAL(clicked()), this, SLOT(btn_look_clicked()));

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
    connect(Btn_change,     SIGNAL(clicked()), this, SLOT(btn_change_clicked()));     //смена фамилий
    connect(BtnSemiFinal,   SIGNAL(clicked()), this, SLOT(btn_semiFinal_clicked()));
    connect(BtnFinal,       SIGNAL(clicked()), this, SLOT(btn_final_clicked()));

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

    connect(pult2, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult2, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult3, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult3, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult4, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult4, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

    connect(pult5, SIGNAL(pultData(int, int)),  this, SLOT(pult(int, int)));
    connect(pult5, SIGNAL(isConnect(int, int)), this, SLOT(pult_off(int, int)));

//    connect(sett->LED_CITY,          SIGNAL(editingFinished()), this, SLOT(editSity()));
//    connect(sett->NAME_COMPETITION,  SIGNAL(editingFinished()), this, SLOT(editSity()));
//    connect(sett->DATEEDIT,          SIGNAL(editingFinished()), this, SLOT(editSity()));

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

    //connect(btnChoice, SIGNAL(clicked()), this, SLOT(btnChoice_clicked()));

    /////////////////////////////////////////
    /// Анимация открытия окна с камерами ///
    /////////////////////////////////////////
    animOpen = new QPropertyAnimation(windowCameras, "geometry", this);
    animOpen->setObjectName("animOpen");
    animClose = new QPropertyAnimation(windowCameras, "geometry", this);

    icoOpen = new QIcon(":/images/open.png");
    icoClose = new QIcon(":/images/close.png");
    btnCameras->setIcon(*icoOpen);

    animOpen->setStartValue(QRect(windowCameras->geometry().x(), 876, windowCameras->geometry().width(), 21));
    animOpen->setEndValue(QRect(windowCameras->geometry().x(), 410, windowCameras->geometry().width(), 487));
    animOpen->setDuration(300);

    animClose->setStartValue(QRect(windowCameras->geometry().x(), 410, windowCameras->geometry().width(), 487));
    animClose->setEndValue(QRect(windowCameras->geometry().x(), 876, windowCameras->geometry().width(), 21));
    animClose->setDuration(300);

    connect(btnCameras, SIGNAL(clicked()), this, SLOT(animation()));
    connect(animOpen, SIGNAL(finished()), this, SLOT(endAnimation()));
    connect(animClose, SIGNAL(finished()), this, SLOT(endAnimation()));
    windowCameras->setGeometry(windowCameras->geometry().x(), 876, windowCameras->geometry().width(), 21);
    ///////////////////////////////////////////////////////////

    leCam1->setObjectName("leCam1");
    leCam2->setObjectName("leCam2");
    //leCam3->setObjectName("leCam3");
    connect(leCam1, SIGNAL(editingFinished()), this, SLOT(setCam()));
    connect(leCam2, SIGNAL(editingFinished()), this, SLOT(setCam()));
    connect(leCam3, SIGNAL(editingFinished()), this, SLOT(setCam()));

    connect(cbAutoCam1, SIGNAL(toggled(bool)), this, SLOT(autoCamera(bool)));
    connect(cbAutoCam2, SIGNAL(toggled(bool)), this, SLOT(autoCamera(bool)));
    connect(cbAutoCam3, SIGNAL(toggled(bool)), this, SLOT(autoCamera(bool)));

    QFile cam1("cam1.txt");
    if(!cam1.exists()){
        cam1.open(QIODevice::WriteOnly);
        cam1.close();
    }
    else{
        cam1.open(QIODevice::ReadOnly);
        cam1Url = cam1.readAll();
        leCam1->setText(cam1Url);
        cam1.close();
    }

    QFile cam2("cam2.txt");
    if(!cam2.exists()){
        cam2.open(QIODevice::WriteOnly);
        cam2.close();
    }
    else{
        cam2.open(QIODevice::ReadOnly);
        cam2Url = cam2.readAll();
        leCam2->setText(cam2Url);
        cam2.close();
    }

    QFile cam3("cam3.txt");
    if(!cam3.exists()){
        cam3.open(QIODevice::WriteOnly);
        cam3.close();
    }
    else{
        cam3.open(QIODevice::ReadOnly);
        cam3Url = cam3.readAll();
        leCam3->setText(cam3Url);
        cam3.close();
    }


}

void FormMain::animation(){
    if(windowCameras->geometry().height() == 21)
        animOpen->start();
    else
        animClose->start();
}

void FormMain::endAnimation(){
    if(sender()->objectName() == "animOpen")
        btnCameras->setIcon(*icoClose);
    else
        btnCameras->setIcon(*icoOpen);
}

void FormMain::autoCamera(bool state){
    qDebug()<<state<<sender()->objectName();
    if(state){
        if(sender()->objectName() == "cbAutoCam1"){
            cbAutoCam2->setEnabled(false);
            cbAutoCam3->setEnabled(false);
            camConn = new CameraConnection(this);
            leCam1->setText("");
            leCam1->setStyleSheet("background-color: red");
        }
        else if(sender()->objectName() == "cbAutoCam2"){
            cbAutoCam1->setEnabled(false);
            cbAutoCam3->setEnabled(false);
            camConn = new CameraConnection(this, 2);
            leCam2->setText("");
            leCam2->setStyleSheet("background-color: red");
        }
        else{
            cbAutoCam1->setEnabled(false);
            cbAutoCam2->setEnabled(false);
            camConn = new CameraConnection(this, 2);
            leCam3->setText("");
            leCam3->setStyleSheet("background-color: red");
        }
        connect(camConn, SIGNAL(sigCamera(QString)), this, SLOT(setCamera(QString)));
    }
    else{
        if(sender()->objectName() == "cbAutoCam1"){
            cbAutoCam2->setEnabled(true);
            cbAutoCam3->setEnabled(true);
            leCam1->setStyleSheet("background-color: white");
            QFile f("cam1.txt");
            f.open(QIODevice::ReadOnly);
            leCam1->setText(f.readLine());
            f.close();
        }
        else if(sender()->objectName() == "cbAutoCam2"){
            cbAutoCam1->setEnabled(true);
            cbAutoCam3->setEnabled(true);
            leCam2->setStyleSheet("background-color: white");
            QFile f("cam2.txt");
            f.open(QIODevice::ReadOnly);
            leCam2->setText(f.readLine());
            f.close();
        }
        else{
            cbAutoCam1->setEnabled(true);
            cbAutoCam2->setEnabled(true);
            leCam3->setStyleSheet("background-color: white");
            QFile f("cam3.txt");
            f.open(QIODevice::ReadOnly);
            leCam3->setText(f.readLine());
            f.close();
        }
        if(camConn){
            disconnect(camConn, SIGNAL(sigCamera(QString)), nullptr, nullptr);
            camConn->deleteLater();
        }
    }
}

void FormMain::setCamera(QString ip){
    qDebug()<<"ip = "<<ip;
    if(cbAutoCam1->isChecked()){
        QFile f;
        QTextStream out(&f);
        f.setFileName("cam1.txt");
        f.open(QIODevice::WriteOnly);
        out << "srt://" + ip + ":1111";
        f.close();
        cam1Url = "srt://" + ip + ":1111";
        cbAutoCam1->setChecked(false);
    }
    if(cbAutoCam2->isChecked()){
        QFile f;
        QTextStream out(&f);
        f.setFileName("cam2.txt");
        f.open(QIODevice::WriteOnly);
        out << "srt://" + ip + ":2222";
        f.close();
        cam2Url = "srt://" + ip + ":2222";
        cbAutoCam2->setChecked(false);
    }
    if(cbAutoCam3->isChecked()){
        QFile f;
        QTextStream out(&f);
        f.setFileName("cam3.txt");
        f.open(QIODevice::WriteOnly);
        out << "srt://" + ip + ":3333";
        f.close();
        cam3Url = "srt://" + ip + ":3333";
        cbAutoCam3->setChecked(false);
    }
}

void FormMain::setCam(){
    QFile f;
    QTextStream out(&f);
    if(sender()->objectName() == "leCam1"){
        f.setFileName("cam1.txt");
        f.open(QIODevice::WriteOnly);
        out << leCam1->text();
        cam1Url = leCam1->text();
    }else if(sender()->objectName() == "leCam2"){
        f.setFileName("cam2.txt");
        f.open(QIODevice::WriteOnly);
        out << leCam2->text();
        cam2Url = leCam2->text();
    }else{
        f.setFileName("cam3.txt");
        f.open(QIODevice::WriteOnly);
        out << leCam3->text();
        cam3Url = leCam3->text();
    }
    f.close();
}

//////////////////////////////////
// функция нахождения id раунда //
//      и текущего режима       //
//////////////////////////////////

void FormMain::find_id_round(void){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        qDebug() << "Error: connection with database failed";
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (find_id_round)!");
        msgBox.exec();
        return;
    }

    QString a = cmbAge->currentText();                      // возраст
    QString w = cmbWeight->currentText();                   // вес
    qDebug() << cmb_round->currentText();
    QSqlQuery query;
    QString sql;
    if(cmb_round->currentText() == "Полуфинал"){
        sql = "SELECT id FROM rounds WHERE age = '%1' and weight = '%2' and mode = 2";
        sql = sql.arg(a).arg(w);
    }else if(cmb_round->currentText() == "Финал"){
        sql = "SELECT id FROM rounds WHERE age = '%1' and weight = '%2' and mode = 3";
        sql = sql.arg(a).arg(w);
    }
    else{
        int r = cmb_round->currentText().remove(0, 5).toInt();  // номер круга
        sql = "SELECT id FROM rounds WHERE age = '%1' and weight = '%2' and round = '%3'";
        sql = sql.arg(a).arg(w).arg(r);
    }
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
        if(current_mode == 1 || current_mode == 2 || current_mode == 3){
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
    m_db.close();
}

///////////////////////////////
//   выбор нового возраста   //
///////////////////////////////
void FormMain::age(void){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (age)!");
        msgBox.exec();
        return;
    }
    QString age = cmbAge->currentText();
    flag_shoise = 1;
    disconnect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    disconnect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
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

    sql = "SELECT round, mode FROM rounds WHERE age = '%1' and weight = '%2'";
    sql = sql.arg(age).arg(cmbWeight->currentText());
    l.clear();
    query.exec(sql);
    qDebug() << "age 2";
    while(query.next()){
        l.append(query.value(0).toString() + ";" + query.value(1).toString());
        qDebug() << "value 12";
    }
    //std::sort(l.begin(), l.end());
    l = f_sort(l);
    cmb_round->clear();
    foreach(QString sEach, l){
        if(sEach.split(";")[1] == "2")
            cmb_round->addItem("Полуфинал");
        else if(sEach.split(";")[1] == "3")
            cmb_round->addItem("Финал");
        else
            cmb_round->addItem("круг " + sEach.split(";")[0]);
    }

    cmb_round->setCurrentIndex(l.length() - 1);
    find_id_round();
    flag_shoise = 0;
    m_db.close();
    connect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
}

//////////////////////////////////////////
//          выбор нового веса           //
//////////////////////////////////////////
void FormMain::weight(void){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        qDebug() << "Error: connection with database failed";
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (weight)!");
        msgBox.exec();
        return;
    }
    QString weight = cmbWeight->currentText();
    if(flag_shoise == 0){
        flag_shoise = 1;
        QString sql_round = "SELECT round, mode FROM rounds WHERE age = '%1' and weight = '%2'";
        sql_round = sql_round.arg(cmbAge->currentText()).arg(weight);
        QSqlQuery query;
        query.exec(sql_round);
        qDebug() << "weight 1";
        QList<QString> l;
        while(query.next()){
            l.append(query.value(0).toString() + ";" + query.value(1).toString());
            qDebug() << "value 13";
        }
        //std::sort(l.begin(), l.end());
        l = f_sort(l);
        disconnect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
        cmb_round->clear();
        foreach(QString sEach, l){
            if(sEach.split(";")[1] == "2")
                cmb_round->addItem("Полуфинал");
            else if(sEach.split(";")[1] == "3")
                cmb_round->addItem("Финал");
            else
                cmb_round->addItem("круг " + sEach.split(";")[0]);
        }

        cmb_round->setCurrentIndex(l.length() - 1);
        find_id_round();
        flag_shoise = 0;
    }
    m_db.close();
    connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
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
    if(cmbWeight->count() == 0 || cmbAge->count() == 0)
        return;
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        qDebug() << "Error: connection with database failed";
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (new_round)!");
        msgBox.exec();
        return;
    }
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
    m_db.close();
}

void FormMain::btn_semiFinal_clicked(void){
    if(cmbWeight->count() == 0 || cmbAge->count() == 0 || cmb_round->currentText() == "Полуфинал" || cmb_round->currentText() == "Финал")
        return;
    QSqlDatabase m_db;
    QMessageBox msgBox;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных (btn_semiFinal_clicked)!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    QString sql = "SELECT * FROM rounds WHERE age = '%1' and weight = '%2' and mode = 2";
    sql = sql.arg(cmbAge->currentText(), cmbWeight->currentText());
    if(!query.exec(sql))
        qDebug()<<"err = "<<query.lastError();
    while(query.next()){
        msgBox.setText("Полуфинальный круг уже создан!");
        msgBox.exec();
        m_db.close();
        return;
    }

    sql = "SELECT id FROM rounds WHERE round = '%1' and age = '%2' and weight = '%3'";
    sql = sql.arg(cmb_round->currentText().split(" ")[1], cmbAge->currentText(), cmbWeight->currentText());
    query.exec(sql);
    if(!query.next()){
        msgBox.setText("Невозможно создать полуфинальный круг!(1)" + query.lastError().text());
        msgBox.exec();
        m_db.close();
        return;
    }

    sql = "SELECT id, id_sportsmen, place FROM errors_and_rates WHERE id_round = %1 and place IN (1, 2, 3, 4) ORDER BY place";
    sql = sql.arg(query.value(0).toString());
    qDebug()<<sql;
    query.exec(sql);
    QStringList list;
    while(query.next())
        list.append(query.value(2).toString());
     list.removeDuplicates();
    if(list.count() != 4){
        msgBox.setText("Ошибка! Для создания полуфинального круга необходимо, чтобы в текущем кругу было 4 спортсмена с проставленными занятыми местами");
        msgBox.exec();
        m_db.close();
        return;
    }
    /* список id спортсменов круга в порядке занятых мест */
    list.clear();
    query.first();
    list.append(query.value(1).toString());
    while(query.next())
        list.append(query.value(1).toString());

    /* определение общего количества кругов в категории ...*/
    sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
    sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
    query.exec(sql);
    QList<QString> l_round;
    while(query.next())
        l_round.append(query.value(0).toString());
    int len_rounds = l_round.length();

    /* ... для присваивания номера следующему кругу */
    sql = "INSERT INTO rounds (round, age, weight, mode) VALUES ('%1', '%2', '%3', '%4')";
    sql = sql.arg(len_rounds + 1).arg(cmbAge->currentText()).arg(cmbWeight->currentText()).arg("2");
    query.exec(sql);

    int last_id = query.lastInsertId().toInt();

    sql = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
    query.exec(sql.arg(list.at(0), QString::number(last_id)));   // пара 1
    query.exec(sql.arg(list.at(3), QString::number(last_id)));   // 1-4
    query.exec(sql.arg(list.at(1), QString::number(last_id)));   // пара 2
    query.exec(sql.arg(list.at(2), QString::number(last_id)));   // 2-3

    m_db.close();

    cmb_round->addItem("Полуфинал");
    cmb_round->setCurrentIndex(len_rounds);

}

void FormMain::btn_final_clicked(void){
    if(cmbWeight->count() == 0 || cmbAge->count() == 0 || cmb_round->currentText() == "Финал")
        return;
    QSqlDatabase m_db;
    QMessageBox msgBox;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных (btn_semiFinal_clicked)!");
        msgBox.exec();
        return;
    }

    int len_rounds = 0;
    QSqlQuery query;
    QString sql = "SELECT * FROM rounds WHERE age = '%1' and weight = '%2' and mode = 3";
    sql = sql.arg(cmbAge->currentText(), cmbWeight->currentText());
    if(!query.exec(sql))
        qDebug()<<"err = "<<query.lastError();
    while(query.next()){
        msgBox.setText("Финальный круг уже создан!");
        msgBox.exec();
        m_db.close();
        return;
    }

    if(cmb_round->currentText() == "Полуфинал"){
        sql = "SELECT id FROM rounds WHERE mode = 2 and age = '%2' and weight = '%3'";
        sql = sql.arg(cmbAge->currentText(), cmbWeight->currentText());
        query.exec(sql);
        if(!query.next()){
            msgBox.setText("Невозможно создать финальный круг!(1)" + query.lastError().text());
            msgBox.exec();
            m_db.close();
            return;
        }

        sql = "SELECT id, id_sportsmen, place FROM errors_and_rates WHERE id_round = %1 ORDER BY id";
        sql = sql.arg(query.value(0).toString());
        query.exec(sql);
        QStringList list;
        int place1, place2, place3, place4;
        bool flag_error = false;
        QList<int> id_sportsmen{0, 0, 0, 0};    //id спортсменов для распределения в финале
        query.next();
        if(!query.value(2).isNull()){
            place1 = query.value(2).toInt();
            if(place1 != 1) flag_error = true;
            id_sportsmen[2] = query.value(1).toInt();
        }
        else{
            place1 = 0;
            id_sportsmen[0] = query.value(1).toInt();
        }
        query.next();
        if(!query.value(2).isNull()){
            place2 = query.value(2).toInt();
            if(place2 != 1) flag_error = true;
            id_sportsmen[2] = query.value(1).toInt();
        }
        else{
            place2 = 0;
            id_sportsmen[0] = query.value(1).toInt();
        }
        query.next();
        if(!query.value(2).isNull()){
            place3 = query.value(2).toInt();
            if(place3 != 1) flag_error = true;
            id_sportsmen[3] = query.value(1).toInt();
        }
        else{
            place3 = 0;
            id_sportsmen[1] = query.value(1).toInt();
        }
        query.next();
        if(!query.value(2).isNull()){
            place4 = query.value(2).toInt();
            if(place4 != 1) flag_error = true;
            id_sportsmen[3] = query.value(1).toInt();
        }
        else{
            place4 = 0;
            id_sportsmen[1] = query.value(1).toInt();
        }

        if((place1 + place2) != 1) flag_error = true;
        if((place3 + place4) != 1) flag_error = true;

        if(flag_error){
            msgBox.setText("Ошибка! Для создания финального круга необходимо, чтобы в каждой полуфинальной подгруппе был отмечен один победитель (цифра 1)");
            msgBox.exec();
            m_db.close();
            return;
        }

        /* определение общего количества кругов в категории ...*/
        sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
        sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
        query.exec(sql);
        QList<QString> l_round;
        while(query.next())
            l_round.append(query.value(0).toString());
        len_rounds = l_round.length();

        /* ... для присваивания номера следующему кругу */
        sql = "INSERT INTO rounds (round, age, weight, mode) VALUES ('%1', '%2', '%3', '%4')";
        sql = sql.arg(len_rounds + 1).arg(cmbAge->currentText()).arg(cmbWeight->currentText()).arg("3");
        query.exec(sql);

        int last_id = query.lastInsertId().toInt();

        sql = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
        query.exec(sql.arg(QString::number(id_sportsmen[0]), QString::number(last_id)));   // пара 1
        query.exec(sql.arg(QString::number(id_sportsmen[1]), QString::number(last_id)));   // за 3 место
        query.exec(sql.arg(QString::number(id_sportsmen[2]), QString::number(last_id)));   // пара 2
        query.exec(sql.arg(QString::number(id_sportsmen[3]), QString::number(last_id)));   // за 1 место

        m_db.close();
        cmb_round->addItem("Финал");
        cmb_round->setCurrentIndex(len_rounds);
    }
    else{
        /* Если в категории два или три спортсмена */
        sql = "SELECT id FROM sportsmens WHERE age = '%1' and weight = '%2'";
        sql = sql.arg(cmbAge->currentText(), cmbWeight->currentText());
        query.exec(sql);
        int count = 0;
        while(query.next())
            count++;
        if(count == 2){
            /* определение общего количества кругов в категории ...*/
            sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
            sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
            query.exec(sql);
            QList<QString> l_round;
            while(query.next())
                l_round.append(query.value(0).toString());
            len_rounds = l_round.length();

            /* ... для присваивания номера следующему кругу */
            sql = "INSERT INTO rounds (round, age, weight, mode) VALUES ('%1', '%2', '%3', '%4')";
            sql = sql.arg(len_rounds + 1).arg(cmbAge->currentText()).arg(cmbWeight->currentText()).arg("3");
            query.exec(sql);

            int last_id = query.lastInsertId().toInt();

            sql = "SELECT id FROM sportsmens WHERE age = '%1' and weight = '%2'";
            sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
            query.exec(sql);
            QList<int> id_sportsmen;
            query.next();
            id_sportsmen.append(query.value(0).toInt());
            query.next();
            id_sportsmen.append(query.value(0).toInt());

            sql = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
            query.exec(sql.arg(QString::number(-1), QString::number(last_id)));                 // пусто
            query.exec(sql.arg(QString::number(-2), QString::number(last_id)));                 // пусто
            query.exec(sql.arg(QString::number(id_sportsmen[0]), QString::number(last_id)));    // пара за;
            query.exec(sql.arg(QString::number(id_sportsmen[1]), QString::number(last_id)));    // 1 место;

            m_db.close();
            cmb_round->addItem("Финал");
            cmb_round->setCurrentIndex(len_rounds);
        }
        else if(count == 3){
            sql = "SELECT id FROM rounds WHERE round = '%1' and age = '%2' and weight = '%3'";
            sql = sql.arg(cmb_round->currentText().split(" ")[1], cmbAge->currentText(), cmbWeight->currentText());
            query.exec(sql);
            if(!query.next()){
                msgBox.setText("Невозможно создать финальный круг!(1)" + query.lastError().text());
                msgBox.exec();
                m_db.close();
                return;
            }

            sql = "SELECT id, id_sportsmen, place FROM errors_and_rates WHERE id_round = %1 and place IN (1, 2, 3) ORDER BY place";
            sql = sql.arg(query.value(0).toString());
            qDebug()<<sql;
            query.exec(sql);
            QStringList list;
            while(query.next())
                list.append(query.value(2).toString());
             list.removeDuplicates();
            if(list.count() != 3){
                msgBox.setText("Ошибка! Для создания финального круга необходимо, чтобы в текущем кругу было 3 спортсмена с проставленными занятыми местами");
                msgBox.exec();
                m_db.close();
                return;
            }
            /* список id спортсменов круга в порядке занятых мест */
            list.clear();
            query.first();
            list.append(query.value(1).toString());
            while(query.next())
                list.append(query.value(1).toString());

            /* определение общего количества кругов в категории ...*/
            sql = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
            sql = sql.arg(cmbAge->currentText()).arg(cmbWeight->currentText());
            query.exec(sql);
            QList<QString> l_round;
            while(query.next())
                l_round.append(query.value(0).toString());
            len_rounds = l_round.length();

            /* ... для присваивания номера следующему кругу */
            sql = "INSERT INTO rounds (round, age, weight, mode) VALUES ('%1', '%2', '%3', '%4')";
            sql = sql.arg(len_rounds + 1).arg(cmbAge->currentText()).arg(cmbWeight->currentText()).arg("3");
            query.exec(sql);

            int last_id = query.lastInsertId().toInt();

            sql = "INSERT INTO errors_and_rates (id_sportsmen, id_round) VALUES ('%1', '%2')";
            query.exec(sql.arg(QString::number(-1), QString::number(last_id))); // пусто
            query.exec(sql.arg(list.at(2), QString::number(last_id)));          // 3 место
            query.exec(sql.arg(list.at(0), QString::number(last_id)));          // за 1
            query.exec(sql.arg(list.at(1), QString::number(last_id)));          // место

            m_db.close();
            cmb_round->addItem("Финал");
            cmb_round->setCurrentIndex(len_rounds);
        }



    }




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
    qDebug()<<rate<<port;
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
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    if(!m_db.open()){
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (btn_enterName_clicked)!");
        msgBox.exec();
        return;
    }
    if(dialog(this) == 0)
        return;

    QStringList data;
    if(cmb_round->currentText() == "Полуфинал"){

    }
    else{
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

        if(!query.exec(sql))
            qDebug() << "error sel name, region";
        else{
            while(query.next()){
                data.append(query.value(0).toString() + ";" + query.value(1).toString());
                qDebug() << "value 2";
            }
        }
    }



    if(flag_mode == 0){
        Choice_one_athlete* ch = new Choice_one_athlete(data, this);    // окно выбора спортсменов общего круга
        total_sum = 0.0;
        QString s;
        s = s.setNum(total_sum);
        lbl_total->setText(s);
    }else{
        if(current_mode == 1 || current_mode == -1)
            Choice_two_athletes* ch = new Choice_two_athletes(data, this);
        else if(current_mode == 2 || current_mode == 3){
            dlgSemiFinal* dlg = new dlgSemiFinal(this, current_mode);
            dlg->exec();
        }
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
    mainwin->menuBar()->setEnabled(true);
    flag_priem = 0;
    m_db.close();
    Btn_change->setEnabled(false);
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
    if(currentDataBase == "")
        return;
    Protocol* prt = new Protocol(this);
    prt->exec();
}

void FormMain::btn_change_clicked(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QMessageBox msgBox;
    m_db.setDatabaseName(currentDataBase);
    if (!m_db.open()){
        msgBox.setText("Ошибка открытия базы данных btn_change_clicked " + m_db.lastError().text());
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    QString sql;
    int id_red;
    int id_blue;
    QString id_sportsmen_red;
    QString id_sportsmen_blue;

    if(current_mode == 2 || current_mode == 3){
        sql = "SELECT err.id, err.id_sportsmen FROM errors_and_rates err "
              "JOIN sportsmens sp ON err.id_sportsmen=sp.id "
              "WHERE err.id_round = '%1' and sp.name = '%2'";
        if(!query.exec(sql.arg(QString::number(id_round), Lbl_name_red->text().split(";")[0]))) qDebug()<<"err select id red = "<<query.lastError()<<sql;
        query.next();
        id_red = query.value(0).toInt();
        id_sportsmen_red = query.value(1).toString();
        sql = "SELECT err.id, err.id_sportsmen FROM errors_and_rates err "
              "JOIN sportsmens sp ON err.id_sportsmen=sp.id "
              "WHERE id_round = '%1' and sp.name = '%2'";
        if(!query.exec(sql.arg(QString::number(id_round), Lbl_name_blue->text().split(";")[0]))) qDebug()<<"err select id blue = "<<query.lastError()<<sql;
        query.next();
        id_blue = query.value(0).toInt();
        id_sportsmen_blue = query.value(1).toString();
    }
    else{
        sql = "SELECT MAX(id) FROM errors_and_rates WHERE id_round = '%1'";     //выбор последней записи в
                                                                                //таблице оценок для текущего круга
        sql = sql.arg(id_round);
        query.exec(sql);
        if(!query.next())
            return;
        id_red = query.value(0).toInt();    //id записи с оценками красного
        id_blue = id_red - 1;               //id записи с оценками синего
        sql = "SELECT id_sportsmen FROM errors_and_rates WHERE id = '%1'";
        query.exec(sql.arg(QString::number(id_red)));
        query.next();
        id_sportsmen_red = query.value(0).toString();
        query.exec(sql.arg(QString::number(id_blue)));
        query.next();
        id_sportsmen_blue = query.value(0).toString();
    }


    sql = "UPDATE errors_and_rates SET id_sportsmen = '%1' WHERE id = '%2'";
    query.exec(sql.arg("-", QString::number(id_blue)));
    if(!query.exec(sql.arg(id_sportsmen_blue, QString::number(id_red))))
        qDebug()<<"error update red"<<query.lastError();
    if(!query.exec(sql.arg(id_sportsmen_red, QString::number(id_blue))))
        qDebug()<<"error update blue"<<query.lastError();
    m_db.close();
    QString temp = Lbl_name_red->text();
    Lbl_name_red->setText(Lbl_name_blue->text());
    fr2->name_red->Text(Lbl_name_red->text().split(";")[0]);
    fr2->region_red->Text(Lbl_name_red->text().split(";")[1]);
    Lbl_name_blue->setText(temp);
    fr2->name_blue->Text(Lbl_name_blue->text().split(";")[0]);
    fr2->region_blue->Text(Lbl_name_blue->text().split(";")[1]);
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
///////////////////////////////////////////////////////////////
/// Вызов формы добавления спортсмена из меню главного окна ///
///////////////////////////////////////////////////////////////
void FormMain::addOneSportsmen(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных addOneSportsmen!");
        msgBox.exec();
        return;
    }
    QDialog* dlg = new QDialog;
    Ui::dlgAddOne ui;
    ui.setupUi(dlg);
    QString sql_age     = "SELECT DISTINCT (age) FROM sportsmens";
    QString sql_weight  = "SELECT DISTINCT (weight) FROM sportsmens";
    QSqlQuery query;
    query.exec(sql_age);
    if(query.next()){
        query.previous();
        while(query.next()){
            qDebug()<<query.value(0).toString();
            ui.cbAge->addItem(query.value(0).toString());
        }
    }
    query.exec(sql_weight);
    if(query.next()){
        query.previous();
        while(query.next()){
            qDebug()<<query.value(0).toString();
            ui.cbWeight->addItem(query.value(0).toString());
        }
    }
    query.exec("SELECT DISTINCT (region) FROM sportsmens");
    if(query.next()){
        query.previous();
        while(query.next()){
            //qDebug()<<query.value(0).toString();
            ui.cbTeam->addItem(query.value(0).toString());
        }
    }
    m_db.close();
    int ret = dlg->exec();
    if(ret){
        if(ui.leName->text().trimmed() == "" ||
           ui.cbAge->currentText().trimmed() == "" ||
           ui.cbTeam->currentText().trimmed() == ""){
            QMessageBox msgBox;
            msgBox.setText("Все поля должны быть заполнены!");
            msgBox.exec();
            return;
        }
        QClipboard* pcb = QApplication::clipboard();
        pcb->setText(ui.leName->text() + "\t" + ui.cbTeam->currentText() + "\t" +
                     ui.cbAge->currentText() + "\t" + ui.cbWeight->currentText());
        addSportsmenFromBuffer();
    }
}

///////////////////////////////////////////////////////////////
/// Вызов формы добавления спортсмена из меню главного окна ///
///////////////////////////////////////////////////////////////
void FormMain::addOneReferee(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных addOneReferee!");
        msgBox.exec();
        return;
    }
    QDialog* dlg = new QDialog;
    Ui::dlgAddOne ui;
    ui.setupUi(dlg);
    ui.lblRegion->setText("Регион");
    ui.cbAge->setEnabled(false);
    ui.cbWeight->setEnabled(false);
    QString sql_region     = "SELECT DISTINCT (region) FROM referee";
    QSqlQuery query;
    query.exec(sql_region);
    if(query.next()){
        query.previous();
        while(query.next()){
            //qDebug()<<query.value(0).toString();
            ui.cbTeam->addItem(query.value(0).toString());
        }
    }
    m_db.close();
    int ret = dlg->exec();
    if(ret){
        if(ui.leName->text().trimmed() == "" ||
           ui.cbTeam->currentText().trimmed() == ""){
            QMessageBox msgBox;
            msgBox.setText("Все поля должны быть заполнены!");
            msgBox.exec();
            return;
        }
        QClipboard* pcb = QApplication::clipboard();
        pcb->setText(ui.leName->text() + "\t" + ui.cbTeam->currentText());
        addRefereesFromBuffer();
    }
}

///////////////////////////////////////////////////////////
/// Вызов формы добавления спортсменов из буфера обмена ///
///////////////////////////////////////////////////////////
void FormMain::addSportsmenFromBuffer(){
    QClipboard* pcb = QApplication::clipboard();
    QList<QString> rowsFromExcel = pcb->text().split("\n");
    QMessageBox msgBox;
    if(rowsFromExcel.count() < 1){
        msgBox.setText("Неверные данные в буфере обмена!");
        msgBox.exec();
        return;
    }
    if(rowsFromExcel.at(0).split("\t").count() < 4){
        msgBox.setText("Неверные данные в буфере обмена!");
        msgBox.exec();
        return;
    }
    QList<QStringList> listStringList;
    foreach(QString str, rowsFromExcel){
        QList<QString> listString = str.split("\t");
        if(listString.count() == 4){
            listStringList.append(listString);
        }
    }
    QDialog addSportsmens;
    addSportsmens.setGeometry(100, 100, 600, 700);
    QTableWidget tbl(listStringList.count(), 4, &addSportsmens);
    tbl.setGeometry(0, 0, 600, 620);
    tbl.setHorizontalHeaderLabels({"Фамилия Имя", "Команда", "Возраст", "Вес"});
    tbl.horizontalHeader()->resizeSection(0, 200);
    tbl.horizontalHeader()->resizeSection(1, 200);
    tbl.horizontalHeader()->resizeSection(2, 75);
    tbl.horizontalHeader()->resizeSection(3, 75);
    int row = 0;
    foreach(QList<QString> listString, listStringList){
        QTableWidgetItem *newItem0 = new QTableWidgetItem(listString.at(0));
        tbl.setItem(row, 0, newItem0);
        QTableWidgetItem *newItem1 = new QTableWidgetItem(listString.at(1));
        tbl.setItem(row, 1, newItem1);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(listString.at(2));
        newItem2->setTextAlignment(Qt::AlignHCenter);
        tbl.setItem(row, 2, newItem2);
        QTableWidgetItem *newItem3 = new QTableWidgetItem(listString.at(3));
        newItem3->setTextAlignment(Qt::AlignHCenter);
        tbl.setItem(row, 3, newItem3);
        row++;
    }

    QPushButton *btnAdd = new QPushButton("Добавить", &addSportsmens);
    QPushButton *btnCancel = new QPushButton("Отмена", &addSportsmens);
    btnCancel->move(500, 650);
    btnAdd->move(400, 650);
    connect(btnCancel, SIGNAL(clicked()), &addSportsmens, SLOT(reject()));
    connect(btnAdd, SIGNAL(clicked()), &addSportsmens, SLOT(accept()));

    if(addSportsmens.exec())
        addSportsmensToBaza(listStringList);
}

/////////////////////////////////////////////////////
/// Вызов формы добавления судей из буфера обмена ///
/////////////////////////////////////////////////////
void FormMain::addRefereesFromBuffer(){
    QClipboard* pcb = QApplication::clipboard();
    QList<QString> rowsFromExcel = pcb->text().split("\n");
    QMessageBox msgBox;
    if(rowsFromExcel.count() < 1){
        msgBox.setText("Неверные данные в буфере обмена!");
        msgBox.exec();
        return;
    }
    if(rowsFromExcel.at(0).split("\t").count() < 2){
        msgBox.setText("Неверные данные в буфере обмена!");
        msgBox.exec();
        return;
    }
    QList<QStringList> listStringList;
    foreach(QString str, rowsFromExcel){
        QList<QString> listString = str.split("\t");
        if(listString.count() == 2){
            listStringList.append(listString);
        }
    }
    QDialog addReferees;
    addReferees.setGeometry(100, 100, 300, 700);
    QTableWidget tbl(listStringList.count(), 2, &addReferees);
    tbl.setGeometry(0, 0, 300, 620);
    tbl.setHorizontalHeaderLabels({"Фамилия Имя", "Регион"});
    tbl.horizontalHeader()->resizeSection(0, 150);
    tbl.horizontalHeader()->resizeSection(1, 120);
    int row = 0;
    foreach(QList<QString> listString, listStringList){
        QTableWidgetItem *newItem0 = new QTableWidgetItem(listString.at(0));
        tbl.setItem(row, 0, newItem0);
        QTableWidgetItem *newItem1 = new QTableWidgetItem(listString.at(1));
        tbl.setItem(row, 1, newItem1);
        row++;
    }

    QPushButton *btnAdd = new QPushButton("Добавить", &addReferees);
    QPushButton *btnCancel = new QPushButton("Отмена", &addReferees);
    btnCancel->move(200, 650);
    btnAdd->move(100, 650);
    connect(btnCancel, SIGNAL(clicked()), &addReferees, SLOT(reject()));
    connect(btnAdd, SIGNAL(clicked()), &addReferees, SLOT(accept()));

    if(addReferees.exec())
        addRefereesToBaza(listStringList);
}

////////////////////////////////////////////
/// Добавление спортсменов в базу данных ///
////////////////////////////////////////////
void FormMain::addSportsmensToBaza(QList<QStringList> lSl){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных addSportsmensToBaza!");
        msgBox.exec();
        return;
    }
    QSqlQuery query;
    foreach(QStringList sL, lSl){
        QString str = "INSERT INTO sportsmens (name, region, age, weight) VALUES ('%1', '%2', '%3', '%4');";
        QString s;
        s =  str.arg(sL[0], sL[1], sL[2], sL[3]);
        if(query.exec(s)) qDebug() << "Filled sportsmen's table" << s;
        else qDebug() << "error filled sportsmen's table" << s;
    }
    /////////////////////////////////
    /// Заполнение таблицы rounds ///
    /////////////////////////////////

    QString sql_age =           "SELECT DISTINCT (age) FROM sportsmens";
    QString sql_weight =        "SELECT DISTINCT weight FROM sportsmens WHERE age LIKE ('%1')";
    QString sql_round =         "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
    QString sql_round_insert =  "INSERT INTO rounds (round, age, weight) VALUES ('%1', '%2', '%3')";
    query.exec(sql_age);

    while(query.next()){
        QString s = sql_weight.arg(query.value(0).toString());
        QSqlQuery query_weight;
        query_weight.exec(s);
        while(query_weight.next()){
            QSqlQuery query_round;
            QString s = sql_round.arg(query.value(0).toString(),query_weight.value(0).toString());
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
    disconnect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
    disconnect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    disconnect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));

    cmbAge->clear();
    cmbWeight->clear();
    cmb_round->clear();
    query.exec("SELECT DISTINCT (age) FROM sportsmens");
    QList<QString> l_age;
    while(query.next())
        l_age.append(query.value(0).toString());
    l_age = f_sort(l_age);
    foreach(QString sEach, l_age)
        cmbAge->addItem(sEach);
    QString sql = "SELECT DISTINCT weight FROM sportsmens WHERE age = '%1'";
    sql = sql.arg(l_age[0]);
    query.exec(sql);
    QList<QString> l_weight;
    while(query.next())
        l_weight.append(query.value(0).toString());
    l_weight = f_sort(l_weight);
    foreach(QString sEach, l_weight)
        cmbWeight->addItem(sEach);
    sql_round = "SELECT round FROM rounds WHERE age = '%1' and weight = '%2'";
    sql_round =sql_round.arg(l_age[0]).arg(l_weight[0]);
    query.exec(sql_round);
    QList<QString> l_round;
    while(query.next())
        l_round.append(query.value(0).toString());
    foreach(QString sEach, l_round)
        cmb_round->addItem("круг " + sEach);
    cmb_round->setCurrentIndex(l_round.count() - 1);
    connect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
    connect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
    m_db.close();
}

//////////////////////////////////////
/// Добавление судей в базу данных ///
//////////////////////////////////////
void FormMain::addRefereesToBaza(QList<QStringList> lSl){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных addRefereesToBaza!");
        msgBox.exec();
        return;
    }
    QSqlQuery query;
    foreach(QStringList sL, lSl){
        QString str = "INSERT INTO referee (name, region) VALUES ('%1', '%2');";
        QString s;
        s =  str.arg(sL[0], sL[1]);
        if(query.exec(s)) qDebug() << "Filled referee's table" << s;
        else qDebug() << "error filled referee's table" << s;
    }

    m_db.close();

    updateReferee();

}

void FormMain::updateReferee(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных addRefereesToBaza!");
        msgBox.exec();
        return;
    }
    QSqlQuery query;
    query.exec("SELECT * FROM referee");
    listReferees.clear();
    while(query.next()){
        listReferees.append({query.value(0).toString(),
                             query.value(1).toString(),
                             query.value(2).toString(),
                             query.value(3).toString(),
                             query.value(4).toString()});
    }

    m_db.close();
}

///////////////////
/// Выбор ковра ///
///////////////////
void FormMain::choiceMats(QString mat){
    //qDebug()<<mat;
    QSettings settings ("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    if (mat == "Ковёр А"){
        port2 = 4120;
        port3 = 4130;
        port4 = 4140;
        port5 = 4150;
        lblMat->setText("A");
        settings.setValue( "mat", "А");
        ret_mat = 1;
    }
    if (mat == "Ковёр Б"){
        port2 = 4220;
        port3 = 4230;
        port4 = 4240;
        port5 = 4250;
        lblMat->setText("Б");
        settings.setValue( "mat", "Б");
        ret_mat = 0;
    }
    pult2->setPort(port2);
    pult3->setPort(port3);
    pult4->setPort(port4);
    pult5->setPort(port5);
    settings.endGroup();
}

/////////////////////
/// Вызов справки ///
/////////////////////
void FormMain::showHelp(){
    qDebug()<<"showHelp";
}

////////////////////////////////////
/// Создание нового соревнования ///
////////////////////////////////////
void FormMain::createCompetition(){
    AddCompetition* add = new AddCompetition(true, mainwin, this);
    add->exec();
    QList<QAction*> allActions = mainwin->findChildren<QAction*>();
    foreach(QAction* act, allActions){
        if(act->text() == currentDataBase.left(currentDataBase.lastIndexOf('.'))){
            act->trigger();
            break;
        }
    }
}

//////////////////////////////////////
/// Открыть последние соревнования ///
//////////////////////////////////////
void FormMain::choiceCompetitions(QString s){
    currentDataBase = s + ".db";
    mainwin->lblStatus->setText(s.replace('_', ' '));
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(currentDataBase);
    mainwin->Edit->setEnabled(true);
    mainwin->Sportsmens->setEnabled(true);
    mainwin->Referees->setEnabled(true);
    QMessageBox msgBox;
    if(!m_db.open()){
        msgBox.setText("Ошибка базы данных choiceCompetitions!");
        msgBox.exec();
        return;
    }
    QSqlQuery query;

    query.exec("SELECT id, name, region FROM referee");
    listReferees.clear();
    while(query.next()){
        listReferees.append({query.value(0).toString(), query.value(1).toString(), query.value(2).toString()});
    }

    //////////////////////////////////////////////////////////
    /*           добавление элементов в комбобоксы          */
    /*               "возраст", "вес" и "круг"              */
    //////////////////////////////////////////////////////////
    query.exec("SELECT DISTINCT (age) FROM sportsmens");
    if(!query.next()){
        disconnect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
        disconnect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
        disconnect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
        cmbAge->clear();
        cmbWeight->clear();
        cmb_round->clear();
        connect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
        connect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
        connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
        m_db.close();
        return;
    }
    query.previous();
    disconnect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
    disconnect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));
    disconnect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));

    cmbAge->clear();
    cmbWeight->clear();
    cmb_round->clear();

    QList<QString> l_age;
    while(query.next())
        l_age.append(query.value(0).toString());
    l_age = f_sort(l_age);
    foreach(QString sEach, l_age)
        cmbAge->addItem(sEach);
    QString sql = "SELECT DISTINCT weight FROM sportsmens WHERE age = '%1'";
    sql = sql.arg(l_age[0]);
    query.exec(sql);
    QList<QString> l_weight;
    while(query.next())
        l_weight.append(query.value(0).toString());
    l_weight = f_sort(l_weight);
    foreach(QString sEach, l_weight)
        cmbWeight->addItem(sEach);
    QString sql_round = "SELECT round, mode FROM rounds WHERE age = '%1' and weight = '%2'";
    sql_round =sql_round.arg(l_age[0]).arg(l_weight[0]);
    query.exec(sql_round);
    QList<QStringList> l_round;
    while(query.next())
        l_round.append({query.value(0).toString(), query.value(1).toString()});
    foreach(QStringList sEach, l_round){
        if(sEach[1] == "2")
            cmb_round->addItem("Полуфинал");
        else if(sEach[1] == "3")
            cmb_round->addItem("Финал");
        else
            cmb_round->addItem("круг " + sEach[0]);
    }

    connect(cmb_round,  SIGNAL(currentIndexChanged(int)), this, SLOT(round_()));
    cmb_round->setCurrentIndex(l_round.count() - 1);
    connect(cmbAge,     SIGNAL(currentIndexChanged(int)), this, SLOT(age()));
    connect(cmbWeight,  SIGNAL(currentIndexChanged(int)), this, SLOT(weight()));

    m_db.close();
}

////////////////////////////
/// Править соревнование ///
////////////////////////////
void FormMain::editCompetition(){
    QDialog* add = new QDialog();
    Ui::AddCompetition ui;
    ui.setupUi(add);
    ui.btnCreate->setText("Изменить");
    connect(ui.btnCancel, SIGNAL(clicked()), add, SLOT(reject()));
    connect(ui.btnCreate, SIGNAL(clicked()), add, SLOT(accept()));
    QStringList s = currentDataBase.split("_");
    ui.leName->setText(s.at(0));
    ui.leCity->setText(s.at(1));
    ui.deStart->setDate(QDate::fromString(s.at(2), "dd.MM.yyyy"));
    QString fin = s.at(3);
    fin.remove(s.at(3).lastIndexOf('.'), 3);
    ui.deFinish->setDate(QDate::fromString(fin, "dd.MM.yyyy"));
    QList<QAction*> lA = mainwin->lastCompetitions->actions();
    if(add->exec()){
        QString oldNameDataBase = currentDataBase;
        QFile file(currentDataBase);
        currentDataBase = ui.leName->text() + "_" + ui.leCity->text() + "_" + ui.deStart->text() + "_" + ui.deFinish->text() + ".db";
        file.rename(currentDataBase);
        QString newNameDataBase = currentDataBase;
        newNameDataBase = newNameDataBase.remove(newNameDataBase.lastIndexOf('.'), 3);
        oldNameDataBase = oldNameDataBase.remove(oldNameDataBase.lastIndexOf('.'), 3);
        mainwin->lblStatus->setText(newNameDataBase);
        foreach(QAction* act, lA){
            if(act->text() == oldNameDataBase)
                act->setText(newNameDataBase);
        }
    }
}
