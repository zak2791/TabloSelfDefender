#ifndef MAIN_H
#define MAIN_H

#include <QtWidgets>
#include <ui_FormMain.h>
#include "settings.h"
#include "firstround.h"
#include "firstround2.h"
#include "errors.h"
#include "udp.h"
#include "showreports.h"

class TabFilter : public QObject {
    Q_OBJECT

public:
      TabFilter(QObject* parent=nullptr);

signals:
      void focus(void);

protected:
    virtual bool eventFilter(QObject*, QEvent*);
};


class FormMain : public QWidget, private Ui::FormMain{
    Q_OBJECT

private:

    //////////////////////////////////////////////////////////
    /* флаг режима -- -1 не определено (при создании круга) */
    /*                 0 общий круг                         */
    /*                 1 попарно                            */
    //////////////////////////////////////////////////////////


                                    // (устанавливается при выборе или создании круга)
    int id_sportsmen;               // текущий спортсмен в режиме общего круга
    int id_red;                     // текущий спортсмен с красным поясом в парном режиме
    int id_blue;                    // текущий спортсмен с синим поясом в парном режиме


    int first_color;                // первый прием: 0-красного спортсмена, 1-синего спортсмена
    int flag_shoise;                // флаг выбора возраста и веса (для отключения множественного вызова функций)


    QString name_competition;       //название соревнования
    QString date;                   //дата соевнования
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////////////////////////////////
     int timer_id;                  // ID таймера для задержки очистки полей ввода оценок

     //----списки с развернутыми оценками судей----//

     QList<QString> lref1_red;
     QList<QString> lref2_red;
     QList<QString> lref3_red;
     QList<QString> lref4_red;
     QList<QString> lref5_red;

     QList<QString> lref1_blue;
     QList<QString> lref2_blue;
     QList<QString> lref3_blue;
     QList<QString> lref4_blue;
     QList<QString> lref5_blue;

     QPushButton* BtnChoice;

     /////////////////////
     /*   выбор ковра   */
     /////////////////////
     boolean keyVariant;     //вариант расладки клавиатуры
     float rate;             //оцена судьи за компьютером
     /*---состояние кнопок---*/
     boolean stateErr1;
     boolean stateErr2;
     boolean stateErr3;
     boolean stateErr4;
     boolean stateErr5;
     boolean stateErr6;
     boolean stateErr7;
     boolean stateErr8;
     boolean stateErr9;
     boolean stateErr10;
     boolean stateErr11;
     boolean stateErr12;
     boolean stateErr13;
     boolean stateErr14;
     boolean stateErr15;
     boolean statePlusOne;
     boolean stateZero;

     int port2;
     int port3;
     int port4;
     int port5;

     int pult2_connect;
     int pult3_connect;
     int pult4_connect;
     int pult5_connect;



     int num_round;             //текущий круг соревнований
     int count_round;           //количество кругов

     QString sity;              //город для протокола

     QString RateShow;          //показ оценок, 0 - независимо, 1 - все сразу

     QFile file;



     QLabel* LBL2;
     QLabel* LBL3;
     QLabel* LBL4;
     QLabel* LBL5;



     void rate_first(float);

     int min_max(void);
     void clear_keys(void);
     void button_num(QString);
     virtual void keyPressEvent(QKeyEvent*);

private slots:
     void focusSet(void);
     void focusReset(void);
     void set_rate1(QString);
     void set_rate2(QString);
     void set_rate3(QString);
     void set_rate4(QString);
     void set_rate5(QString);

public:
     QString Conn2;             //подключение 2-го судьи, 0 - незарегистрирован
     QString Conn3;             //подключение 3-го судьи, 0 - незарегистрирован
     QString Conn4;             //подключение 4-го судьи, 0 - незарегистрирован
     QString Conn5;             //подключение 5-го судьи, 0 - незарегистрирован

    int flag_mode;               // флаг режима: 0-общий, 1-парный
                                 // (устанавливается при нажатии кнопок выбора режима)

    int id_correct;              // текущая запись в таблице ошибок для корректировки
    int id_round;                   // текущий круг

    FormMain(QWidget *parent = nullptr);
    Settings* sett;  //(sity, name_competition, date, Conn2, Conn3, Conn4, Conn5, this)
    //QString mask_to_list(int);

    int ret_mat;

    QString main_refery;       //главный судья
    QString main_secretary;    //главный секретарь
    QString judge1;            //судья 1
    QString judge2;            //судья 2
    QString judge3;            //судья 3
    QString judge4;            //судья 4
    QString judge5;            //судья 5
    QString ts;                //технический секретарь

    FirstRound* fr;
    FirstRound2* fr2;

    QString team_referees;     //бригада судей (1 или 2)

    Errors* err;

    QLabel* LBL_SUM;
    QLabel* LBL_TOTAL;
    QLabel* Lbl_total_red;
    QLabel* Lbl_total_blue;
    QLabel* Lbl_name;
    QLabel*  Lbl_name_red;
    QLabel*  Lbl_name_blue;

    float total_sum;
    QString current_color;          // текущий цвет спортсмена

    float total_sum_red;
    float total_sum_blue;

    int id_rates;                   // текущая запись в таблице оценок

    int flag_priem;            //флаг выполняемого приема: 0-прием не выполняется, 1...5-выполняется прием 1...5

    int id_rates_red;               //
    int id_rates_blue;              //
    int task;
    int current_mode;               // текущий режим: -1-неопределен, 0-общий, 1-парный

    QString style;
    boolean enable_correct;         // флаг разрешения коррекции результатов текущего вопроса

    Udp* pult2;
    Udp* pult3;
    Udp* pult4;
    Udp* pult5;

    QPushButton* Btn_next;
    QPushButton* Btn_enter;
    QGroupBox* GroupBox_2;
    QComboBox* Cmb_round;
    QComboBox* CmbAge;
    QComboBox* CmbWeight;
    QPushButton* Btn_new;

    QLabel* Lbl_sum;
    QLabel* Lbl_q;
    QRadioButton* RbRed;
    QRadioButton* RbBlue;

    QLineEdit* Led1;
    QLineEdit* Led2;
    QLineEdit* Led3;
    QLineEdit* Led4;
    QLineEdit* Led5;

    QList<QString> lref1_red_blue;
    QList<QString> lref2_red_blue;
    QList<QString> lref3_red_blue;
    QList<QString> lref4_red_blue;
    QList<QString> lref5_red_blue;

    QList<QString> lref1;
    QList<QString> lref2;
    QList<QString> lref3;
    QList<QString> lref4;
    QList<QString> lref5;

    QGroupBox* GroupBox;

    QPixmap flag;
    QPixmap flag_red;
    QPixmap flag_blue;


public slots:
    void btn_settings_clicked(void);
    void btn_next_clicked(void);
    void btn_enter_clicked(void);
    void btn_enterName_clicked(void);
    void btn_0_clicked(void);
    void btn_1_clicked(void);
    void btn_2_clicked(void);
    void btn_3_clicked(void);
    void btn_4_clicked(void);
    void btn_5_clicked(void);
    void btn_6_clicked(void);
    void btn_7_clicked(void);
    void btn_8_clicked(void);
    void btn_9_clicked(void);
    void btn_10_clicked(void);
    void btn_11_clicked(void);
    void btn_look_clicked(void);

    void btn_errors_clicked(void);

    void btn_Err1(void);
    void btn_Err2(void);
    void btn_Err3(void);
    void btn_Err4(void);
    void btn_Err5(void);
    void btn_Err6(void);
    void btn_Err7(void);
    void btn_Err8(void);
    void btn_Err9(void);
    void btn_Err10(void);
    void btn_Err11(void);
    void btn_Err12(void);
    void btn_Err13(void);
    void btn_Err14(void);
    void btn_Err15(void);
    void btn_PlusOne(void);
    void btn_Zero(void);
    void btn_Enter(void);

    void pult(int, int);
    void pult_off(int, int);


private slots:
    void editSity(void);
    void new_round(void);
    void find_id_round(void);
    void switch_variant(bool);
    void age(void);
    void weight(void);
    void round_(void);
    virtual void timerEvent(QTimerEvent*);
    void btnChoice_clicked(void);
    void showWindowCameras(void);
    void hideWindowCameras(void);

    //void on_led1_selectionChanged();
};

#endif // MAIN_H
