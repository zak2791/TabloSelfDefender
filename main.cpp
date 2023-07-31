#include "FormMain.h"
#include "mainwin.h"

int main(int argc, char* argv[]){

    //setlocale(LC_ALL, "");



    //QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForLocale(codec);

    qDebug() << "Привет";

    QApplication app(argc, argv);

    FormMain* form = new FormMain();

    MainWin* mWin = new MainWin();

    QObject::connect(mWin->addOne,          SIGNAL(triggered()), form, SLOT(addOneSportsmen()));
    QObject::connect(mWin->AddFromBuffer,   SIGNAL(triggered()), form, SLOT(addSportsmenFromBuffer()));
    QObject::connect(mWin->MatA,            SIGNAL(triggered()), form, SLOT(choiceMatA()));
    QObject::connect(mWin->MatB,            SIGNAL(triggered()), form, SLOT(choiceMatB()));
    QObject::connect(mWin->ShowHelp,        SIGNAL(triggered()), form, SLOT(showHelp()));
    QObject::connect(mWin->New,             SIGNAL(triggered()), form, SLOT(createCompetition()));
    QObject::connect(mWin->Open,            SIGNAL(triggered()), form, SLOT(openCompetition()));
    QObject::connect(mWin->OpenLast,        SIGNAL(triggered()), form, SLOT(lastCompetitions()));

    mWin->setCentralWidget(form);


    //form->show();
    mWin->show();
    mWin->setFixedSize(mWin->size());

    return app.exec();
}
