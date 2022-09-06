#include "choice.h"

choice::choice(QWidget *parent) : QWidget(parent){
    setupUi(this);
    //setWindowModality();
}

void choice::closeEvent(QCloseEvent*){
    delete this;
}

choice::~choice(){
    qDebug() << "delete";
}
