#include "mainwin.h"
#include "ui_mainwin.h"

#include <QDir>
#include <QDebug>

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    addOne          = ui->AddOne;
    AddFromBuffer   = ui->AddFromBuffer;
    //MatA            = ui->MatA;
    //MatB            = ui->MatB;
    ShowHelp        = ui->ShowHelp;
    New             = ui->New;
    //Open            = ui->Open;
    //OpenLast        = ui->OpenLast;
    Edit            = ui->Edit;

    currentActionText = "";
    currentActionMat = "Ковёр А";

    setWindowTitle("Демонстрация техники");

    QActionGroup* grMats = new QActionGroup(this);

    actMatA = grMats->addAction("Ковёр А");
    actMatA->setCheckable(true);
    actMatA->setChecked(true);
    connect(actMatA, SIGNAL(triggered()), this, SLOT(choiceMats()));

    actMatB = grMats->addAction("Ковёр Б");
    actMatB->setCheckable(true);
    connect(actMatB, SIGNAL(triggered()), this, SLOT(choiceMats()));

    ui->Mat->addActions(grMats->actions());


    menu = ui->Competition;
    lastCompetitions = menu->addMenu("Последние соревнования");
    fillLastCompetitions();
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::fillLastCompetitions(){
    QDir dir = QDir::current();
    QFileInfoList lFiles = dir.entryInfoList({"*.db"}, QDir::Files, QDir::Time);
    if(lFiles.count() > 0){
        QActionGroup* gr = new QActionGroup(this);
        foreach(QFileInfo inf, lFiles){
            QAction* act = gr->addAction(inf.completeBaseName());
            act->setCheckable(true);
            connect(act, SIGNAL(triggered()), this, SLOT(choiceCompetitions()));
        }
        lastCompetitions->clear();
        lastCompetitions->addActions(gr->actions());
    }
}

void MainWin::choiceCompetitions(){
    if(static_cast<QAction*>(sender())->text() != currentActionText){
        qDebug()<<static_cast<QAction*>(sender())->text();
        emit sigChoiceCompetitions(static_cast<QAction*>(sender())->text());
    }
    currentActionText = static_cast<QAction*>(sender())->text();
}

void MainWin::choiceMats(){
    if(static_cast<QAction*>(sender())->text() != currentActionMat)
        emit sigChoiceMats(static_cast<QAction*>(sender())->text());
    currentActionMat = static_cast<QAction*>(sender())->text();
}

void MainWin::choiceMat(QString mat){
    if(mat == "А"){
        currentActionMat = "Ковёр А";
        actMatA->setChecked(true);
    }
    if(mat == "Б"){
        currentActionMat = "Ковёр Б";
        actMatB->setChecked(true);
    }
    //emit sigChoiceMats(currentActionMat);
}
