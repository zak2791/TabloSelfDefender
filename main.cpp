#include "FormMain.h"
#include "mainwin.h"

int main(int argc, char* argv[]){

    //setlocale(LC_ALL, "");

    QApplication app(argc, argv);

    MainWin* mWin = new MainWin();

    FormMain* form = new FormMain(mWin);

    QObject::connect(mWin->AddOne,              SIGNAL(triggered()), form, SLOT(addOneSportsmen()));        //добавить спортсмена
    QObject::connect(mWin->AddFromBuffer,       SIGNAL(triggered()), form, SLOT(addSportsmenFromBuffer())); //добавить спортсменов
                                                                                                            //из буфера обмена

    QObject::connect(mWin->AddOneRef,           SIGNAL(triggered()), form, SLOT(addOneReferee()));          //добавить судью
    QObject::connect(mWin->AddRefFromBuffer,    SIGNAL(triggered()), form, SLOT(addRefereesFromBuffer()));  //добавить судей
                                                                                                            //из буфера обмена



    QObject::connect(mWin,                  SIGNAL(sigChoiceMats(QString)), form, SLOT(choiceMats(QString)));   //выбор ковра
    QObject::connect(mWin->ShowHelp,        SIGNAL(triggered()), form, SLOT(showHelp()));                       //вызов справки
    QObject::connect(mWin->New,             SIGNAL(triggered()), form, SLOT(createCompetition()));              //создание соревнования

    QObject::connect(mWin->Edit,            SIGNAL(triggered()), form, SLOT(editCompetition()));                //правка соревнования

    //QObject::connect(mWin,            SIGNAL(triggered()), form, SLOT(editCompetition()));                //правка соревнования


    mWin->setCentralWidget(form);

    mWin->show();
    mWin->setFixedSize(mWin->size());

    return app.exec();
}
