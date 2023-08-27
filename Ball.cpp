#include <QtCore>
#include <QMouseEvent>
#include "Ball.h"
#include <QMessageBox>
#include <QPainter>

Ball::Ball(int _diff,
           QString style,
           QWidget *parent) : QLabel(parent){

    ////////////////////////////////////////////////////////////////////////////////////
    /*        Constructor                                                             */
    /*        diff -- разновидность оценки:                                           */
    /*                                      1 - баллы за выполнение приема            */
    /*                                      2 - сумма баллов за прием                 */
    /*                                      3 - общая сумма баллов за все приемы      */
    ////////////////////////////////////////////////////////////////////////////////////

    rt = 0.0;
//    QFile file(style);
//    file.open(QFile::ReadOnly);
    QString styleSheet; // = file.readAll();
    if(style == "style_yellow.qss"){
        styleSheet = "QLabel {border-radius: 30px; "
                             "border-width: 2px; "
                             "border-style: solid; "
                             "border-color:white; "
                             "background-color: black; "
                             "color:black;}";
    }else{
        styleSheet = "QLabel {border-radius: 20px; "
                             "border-width: 2px; "
                             "border-style: solid; "
                             "border-color:white; "
                             "background-color: black; "
                             "color:white;}";
    }
    setStyleSheet(styleSheet);
    setAutoFillBackground(true);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    diff = _diff;
    timer = new QTimer();   //таймер плавного сброса
    timer->setInterval(50);
    connect(timer, SIGNAL(timeout()), SLOT(on_timeout()));
    opacity = 1.0;
    count_opacity = 60;
};



Ball::~Ball()
{
}

void Ball::sbros(){
    if(!(timer->isActive())){
        count_opacity = 60;
        timer->start();
    }
    //qDebug() << "sbros";
}

void Ball::setRate(QString rate) {

    if(timer->isActive()){
        timer->stop();
        opacity = 1.0;
        /*
        QFile file("style_white.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = file.readAll();
        setStyleSheet(styleSheet);
        */
    }
    setText(rate);
}



void Ball::resizeEvent(QResizeEvent * pe){
    QFont fnt;
    fnt.setWeight(50);
    fnt.setPixelSize(height() *2 );
    QFontMetrics fm = QFontMetrics(fnt);
    QString st;
    if(diff == 1)
        st = "9.0";
    else if(diff == 2)
        st = "30.0";
    else if(diff == 3)
        st = "150.0";
    else
        st = "5";

    while(fm.horizontalAdvance(st) > width() * 0.9){
        int a = fnt.pixelSize();
        a--;
        if(a < 1)
            break;
        fnt.setPixelSize(a);
        fm = QFontMetrics(fnt);
        setFont(fnt);
     }
     while(fm.height() > height()*1.3){
        int a = fnt.pixelSize();
        a--;
        if(a < 1)
            break;
        fnt.setPixelSize(a);
        fm = QFontMetrics(fnt);
        setFont(fnt);
     }
}

void Ball::paintEvent(QPaintEvent* pe){
        QPainter qp;
        qp.begin(this);
        qp.setOpacity(opacity);
        qp.drawText(rect(), Qt::AlignCenter, text());
        qp.end();
}

void Ball::on_timeout(){
    count_opacity--;
    if (count_opacity == 0){
        timer->stop();
        setText("");
        opacity = 1.0;
        //QFile file("style_white.qss");
        //file.open(QFile::ReadOnly);
        QString styleSheet = "QLabel {border-radius: 20px; "
                                     "border-width: 2px; "
                                     "border-style: solid; "
                                     "border-color:white; "
                                     "background-color: black; "
                                     "color:white;}";
        setStyleSheet(styleSheet);

    }else{
        opacity = static_cast<float>(count_opacity) / 60.0;
        //qDebug() << "opacity = " << opacity;
    }
    repaint();
}
