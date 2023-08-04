#include "addcompetition.h"
#include "ui_addcompetition.h"
#include <QDebug>

#include <QMessageBox>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStatusBar>

AddCompetition::AddCompetition(bool b, MainWin* mv, QWidget *parent) :
    QDialog(nullptr),
    ui(new Ui::AddCompetition)
{
    ui->setupUi(this);

    formmain = (FormMain*)parent;
    mainwin = mv;

    state = b;
    if(b)
        setWindowTitle("Новое соревнование");
    else
        setWindowTitle("Правка соревнования");
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->btnCreate, SIGNAL(clicked()), this, SLOT(createCompetitions()));
}

AddCompetition::~AddCompetition()
{
    delete ui;

}

void AddCompetition::createCompetitions(){
    QString name = ui->leName->text();
    QString city = ui->leCity->text();
    QString start = ui->deStart->text();
    QString finish = ui->deFinish->text();
    QMessageBox msgBox;
    if(name.trimmed() == "" || city.trimmed() == ""){
        msgBox.setText("Все поля должны быть заполнены!");
        msgBox.exec();
        return;
    }
    QString baza_name = name + "_" + city + "_" + start + "_" + finish + ".db";
    if(QFileInfo::exists(baza_name)){
        msgBox.setText("Соревнование уже существует!");
        msgBox.exec();
        return;
    }
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(baza_name);
    if (!m_db.open()){
        msgBox.setText("Ошибка создания базы данных " + m_db.lastError().text());
        msgBox.exec();
        return;
    }

    QSqlQuery query;

    QString str =   "CREATE TABLE sportsmens "
                    "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, "
                    "region TEXT, age TEXT, weight TEXT, "
                    "UNIQUE (name, region, weight, age))";



    if(!query.exec(str)){
        msgBox.setText("Ошибка создания таблицы sportsmens " + m_db.lastError().text());
        msgBox.exec();
        m_db.close();
        return;
    }

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

    if(!query.exec(str)){
        msgBox.setText("Ошибка создания таблицы errors_and_rates " + m_db.lastError().text());
        msgBox.exec();
        m_db.close();
        return;
    }

    str =           "CREATE TABLE rounds "
                    "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "round INTEGER, age  TEXT, weight TEXT, "
                    "mode INTEGER DEFAULT -1, "
                    "UNIQUE (round, age, weight))";

    if(!query.exec(str)){
        msgBox.setText("Ошибка создания таблицы rounds " + m_db.lastError().text());
        msgBox.exec();
        m_db.close();
        return;
    }
//    QList<QLabel*> lL = mainwin->statusBar()->findChildren<QLabel*>();
//    foreach(QLabel* lbl, lL){
//        mainwin->statusBar()->removeWidget(lbl);
//        delete lbl;
//    }
    //QLabel* lblStatus = new QLabel(baza_name);
    mainwin->lblStatus->setText(baza_name);
    formmain->Cmb_round->clear();
    formmain->CmbAge->clear();
    formmain->CmbWeight->clear();
    //formmain->btnEnterName->setEnabled(false);

    formmain->currentDataBase = baza_name;
    formmain->mainwin->fillLastCompetitions();

    m_db.close();
    reject();
}
