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

    QAction* actMatA = grMats->addAction("Ковёр А");
    actMatA->setCheckable(true);
    actMatA->setChecked(true);
    connect(actMatA, SIGNAL(triggered()), this, SLOT(choiceMats()));

    QAction* actMatB = grMats->addAction("Ковёр Б");
    actMatB->setCheckable(true);
    connect(actMatB, SIGNAL(triggered()), this, SLOT(choiceMats()));

    ui->Mat->addActions(grMats->actions());

    QDir dir = QDir::current();
    QFileInfoList lFiles = dir.entryInfoList({"*.db"}, QDir::Files, QDir::Time);
    if(lFiles.count() > 0){
        QMenu* menu = ui->Competition->addMenu("Последние соревнования");
        QActionGroup* gr = new QActionGroup(this);
        foreach(QFileInfo inf, lFiles){
            QAction* act = gr->addAction(inf.baseName());
            act->setCheckable(true);
            connect(act, SIGNAL(triggered()), this, SLOT(choiceCompetitions()));
        }
        menu->addActions(gr->actions());
    }
}

MainWin::~MainWin()
{
    delete ui;
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

