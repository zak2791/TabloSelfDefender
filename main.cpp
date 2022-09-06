#include "FormMain.h"

int main(int argc, char* argv[]){

    //setlocale(LC_ALL, "");



    //QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForLocale(codec);

    qDebug() << "Привет";

    QApplication app(argc, argv);

    FormMain* form = new FormMain();
    form->show();

    return app.exec();
}
