#include "FormMain.h"
#include "mainwin.h"

int main(int argc, char* argv[]){

    //setlocale(LC_ALL, "");

    QApplication app(argc, argv);

    MainWin* mWin = new MainWin();

    FormMain* form = new FormMain(mWin);

    QObject::connect(mWin->addOne,          SIGNAL(triggered()), form, SLOT(addOneSportsmen()));                //добавить спортсмена
    QObject::connect(mWin->AddFromBuffer,   SIGNAL(triggered()), form, SLOT(addSportsmenFromBuffer()));         //добавить спортсменов
                                                                                                                //из буфера обмена

    QObject::connect(mWin,                  SIGNAL(sigChoiceMats(QString)), form, SLOT(choiceMats(QString)));   //выбор ковра
    QObject::connect(mWin->ShowHelp,        SIGNAL(triggered()), form, SLOT(showHelp()));                       //вызов справки
    QObject::connect(mWin->New,             SIGNAL(triggered()), form, SLOT(createCompetition()));              //создание соревнования

    QObject::connect(mWin->Edit,            SIGNAL(triggered()), form, SLOT(editCompetition()));                //правка соревнования

    mWin->setCentralWidget(form);

    mWin->show();
    mWin->setFixedSize(mWin->size());

    return app.exec();
}
