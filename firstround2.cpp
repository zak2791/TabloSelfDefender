#include "firstround2.h"
#include <QColor>
#include <QPalette>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QPen>
#include <QPainter>
#include "FormMain.h"

FirstRound2::FirstRound2(QWidget* parent):QWidget(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("Демонстрация техники");
    p = parent;
    resize(800,600);
    //setCursor(QtCore.Qt.BlankCursor)
    QPalette palW = palette();
    palW.setColor(QPalette::Window, QColor("black"));
    palW.setColor(QPalette::WindowText, QColor("yellow"));
    setPalette(palW);

    space = 0;

    name_blue = new Fam("white", "");
    region_blue = new Fam("white", "");
    name_red = new Fam("white", "");
    region_red = new Fam("white", "");

    lbl_priem = new QLabel("TASK\n");
    lbl_priem->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_flag_blue = new QLabel("");
    lbl_flag_red = new QLabel("");

    lbl_1 = new QLabel("1");
    lbl_1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_2 = new QLabel("2");
    lbl_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_3 = new QLabel("3");
    lbl_3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_4 = new QLabel("4");
    lbl_4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_5 = new QLabel("5");
    lbl_5->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_sum = new QLabel("SUM");
    lbl_sum->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    s1 = new QLabel;
    s2 = new QLabel;
    s3 = new QLabel;
    s4 = new QLabel;
    s5 = new QLabel;

    lbl_empty_top = new QLabel;
    lbl_empty_bottom = new QLabel;
    space_bottom = new QLabel;
    /*
    lbl_total = new QLabel("TOTAL");
    lbl_total->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lbl_total->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    */

    rate_1 = new Ball(1, "style_white.qss");
    rate_1->setObjectName("rate_1");
    rate_2 = new Ball(1, "style_white.qss");
    rate_2->setObjectName("rate_2");
    rate_3 = new Ball(1, "style_white.qss");
    rate_3->setObjectName("rate_3");
    rate_4 = new Ball(1, "style_white.qss");
    rate_4->setObjectName("rate_4");
    rate_5 = new Ball(1, "style_white.qss");
    rate_5->setObjectName("rate_5");
    rate_sum = new Ball(2, "style_white.qss");
    rate_total_blue = new Ball(3, "style_white.qss");
    rate_total_red = new Ball(3, "style_white.qss");

    count_priem = new Ball(4, "style_yellow.qss");

    judge_flag1 = new QLabel("");
    judge_flag2 = new QLabel("");
    judge_flag3 = new QLabel("");
    judge_flag4 = new QLabel("");
    judge_flag5 = new QLabel("");

    grid = new QGridLayout;
    grid->setSpacing(space);
    //grid.setMargin(11);

    grid->addWidget(name_blue,        0,   0,  4, 34);
    grid->addWidget(name_red,         0,  35,  4, 34);
    grid->addWidget(s2,               4,   0,  1, 36);
    grid->addWidget(region_blue,      5,  17,  4,  9);
    grid->addWidget(lbl_flag_blue,    5,  27,  4,  7);
    grid->addWidget(region_red,       5,  43,  4,  9);
    grid->addWidget(lbl_flag_red,     5,  35,  4,  7);
    grid->addWidget(s3,               9,   0,  3, 36);
    grid->addWidget(rate_total_blue, 12,   6, 10, 18);
    grid->addWidget(lbl_priem,       13,  30,  8,  9);
    grid->addWidget(rate_total_red,  12,  45, 10, 18);
    grid->addWidget(s4,              22,   0,  3, 36);

    //if self.p.ret_num:
    grid->addWidget(lbl_1,            25,  0,  4,  9);
    grid->addWidget(lbl_2,            25, 10,  4,  9);
    grid->addWidget(lbl_3,            25, 20,  4,  9);
    grid->addWidget(lbl_4,            25, 30,  4,  9);
    grid->addWidget(lbl_5,            25, 40,  4,  9);

    grid->addWidget(judge_flag1,      25,  0,  4,  9);
    grid->addWidget(judge_flag2,      25, 10,  4,  9);
    grid->addWidget(judge_flag3,      25, 20,  4,  9);
    grid->addWidget(judge_flag4,      25, 30,  4,  9);
    grid->addWidget(judge_flag5,      25, 40,  4,  9);

    grid->addWidget(rate_1,           29,  0, 10,  9);
    grid->addWidget(rate_2,           29, 10, 10,  9);
    grid->addWidget(rate_3,           29, 20, 10,  9);
    grid->addWidget(rate_4,           29, 30, 10,  9);
    grid->addWidget(rate_5,           29, 40, 10,  9);

    grid->addWidget(lbl_sum,          25, 51,  4, 18);
    grid->addWidget(rate_sum,         29, 51, 10, 18);


    //grid.addWidget(count_priem, 15, 26, 6, 10);

    setLayout(grid);

    judge_flag1->setVisible(false);
    judge_flag2->setVisible(false);
    judge_flag3->setVisible(false);
    judge_flag4->setVisible(false);
    judge_flag5->setVisible(false);

    QDesktopWidget desk2;

    if(QGuiApplication::screens().length() == 1){

        show();
    }else{
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry().width(), 0, 100, 100);
        showFullScreen();
    }
    sbros();
    func();
}

void FirstRound2::sbros(void){
    rate_1->setText("");
    rate_2->setText("");
    rate_3->setText("");
    rate_4->setText("");
    rate_5->setText("");
    rate_sum->setText("");
    rate_total_blue->setText("");
    rate_total_red->setText("");
    count_priem->setText("");
}

void FirstRound2::func(void){
    QFont fnt = QFont();
    fnt.setPixelSize(lbl_priem->height() / 3);
    QFontMetrics fm = QFontMetrics(fnt);
    int a = 0;
    while(fm.horizontalAdvance("TASK") < lbl_priem->width() - lbl_priem->width() * 0.1){
        a = fnt.pixelSize();
        a++;
        if(a > 300)
            break;
        fnt.setPixelSize(a);
        fm = QFontMetrics(fnt);
        lbl_priem->setFont(fnt);
    }
    fnt.setPixelSize(a);
    //lbl_total->setFont(fnt);
    lbl_1->setFont(fnt);
    lbl_2->setFont(fnt);
    lbl_3->setFont(fnt);
    lbl_4->setFont(fnt);
    lbl_5->setFont(fnt);
    lbl_sum->setFont(fnt);
    //lbl_priem->setFont(fnt);
}

void FirstRound2::paintEvent(QPaintEvent*){
    QPainter qp;
    qp.begin(this);
    int w = width();
    QRect rect = lbl_1->geometry();
    int h = rect.top();
    qp.setBrush(QBrush(QColor("blue")));
    qp.drawRect(0, 0, w / 2, h);
    qp.setBrush(QBrush(QColor("red")));
    qp.drawRect(w / 2, 0, w, h);
    qp.end();
}

void FirstRound2::keyPressEvent(QKeyEvent* pe){
    if(pe->key() == Qt::Key_Escape)
        QGuiApplication::quit();
}
