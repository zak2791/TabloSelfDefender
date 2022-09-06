#include "firstround.h"
#include <QColor>
#include <QPalette>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QPen>
#include <QPainter>
#include "FormMain.h"

FirstRound::FirstRound(QWidget* parent):QWidget(parent)
{
    setWindowFlags(Qt::Window);
    p = parent;
    resize(800,600);
    //setCursor(QtCore.Qt.BlankCursor)
    QPalette palW = palette();
    palW.setColor(QPalette::Window, QColor("black"));
    palW.setColor(QPalette::WindowText, QColor("yellow"));
    setPalette(palW);

    name = new Fam("white", "");
    region = new Fam("white", "");

    lbl_priem = new QLabel("TASK");
    lbl_priem->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl_flag = new QLabel("");

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

    lbl_total = new QLabel("TOTAL");
    lbl_total->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lbl_total->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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
    rate_total = new Ball(3, "style_white.qss");

    count_priem = new Ball(4, "style_yellow.qss");

    judge_flag1 = new QLabel("");
    judge_flag2 = new QLabel("");
    judge_flag3 = new QLabel("");
    judge_flag4 = new QLabel("");
    judge_flag5 = new QLabel("");

    grid = new QGridLayout;
    grid->setSpacing(11);
    grid->setMargin(11);

    grid->addWidget(name, 0, 0, 3, 26);
    grid->addWidget(region, 0, 26, 3, 6);
    grid->addWidget(lbl_flag, 0, 32, 3, 4);
    grid->addWidget(lbl_priem, 12, 26, 3, 10);

    grid->addWidget(lbl_1, 3, 0, 3, 5);
    grid->addWidget(lbl_2, 3, 5, 3, 5);
    grid->addWidget(lbl_3, 3, 10, 3, 5);
    grid->addWidget(lbl_4, 3, 15, 3, 5);
    grid->addWidget(lbl_5, 3, 20, 3, 5);

    grid->addWidget(judge_flag1, 3, 0, 3, 5);
    grid->addWidget(judge_flag2, 3, 5, 3, 5);
    grid->addWidget(judge_flag3, 3, 10, 3, 5);
    grid->addWidget(judge_flag4, 3, 15, 3, 5);
    grid->addWidget(judge_flag5, 3, 20, 3, 5);

    grid->addWidget(rate_1, 6, 0, 5, 5);
    grid->addWidget(rate_2, 6, 5, 5, 5);
    grid->addWidget(rate_3, 6, 10, 5, 5);
    grid->addWidget(rate_4, 6, 15, 5, 5);
    grid->addWidget(rate_5, 6, 20, 5, 5);

    grid->addWidget(lbl_sum, 3, 26, 3, 10);

    grid->addWidget(rate_sum, 6, 26, 5, 10);
    grid->addWidget(lbl_total, 12, 0, 3, 25);
    grid->addWidget(rate_total, 15, 5, 6, 15);
    grid->addWidget(count_priem, 15, 26, 6, 10);

    setLayout(grid);

    judge_flag1->setVisible(false);
    judge_flag2->setVisible(false);
    judge_flag3->setVisible(false);
    judge_flag4->setVisible(false);
    judge_flag5->setVisible(false);

    QDesktopWidget desk2;

    if(QGuiApplication::screens().length() == 1){
        QMessageBox* dialog = new QMessageBox(QMessageBox::Information,
                                                "Внимание!", "Для корректной работы программы подключите  внешний \
                                                дисплей и в настройках графики в разделе'Дисплеи' укажите \
                                                'расширенный рабочий стол' или 'дополнительный дисплей(dualwiev). \
                                                Основным дисплеем должен быть указан дисплей ноутбука.'",
                                                QMessageBox::Ok);

        dialog->exec();
        show();
    }else{
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry().width(), 0, 100, 100);
        showFullScreen();
    }
    sbros();
    func();
}

void FirstRound::sbros(void){
    rate_1->setText("");
    rate_2->setText("");
    rate_3->setText("");
    rate_4->setText("");
    rate_5->setText("");
    rate_sum->setText("");
    rate_total->setText("");
    count_priem->setText("");
}

void FirstRound::func(void){
    QFont fnt = QFont();
    fnt.setPixelSize(lbl_total->height() / 3);
    QFontMetrics fm = QFontMetrics(fnt);
    int a = 0;
    while(fm.horizontalAdvance("TOTAL:") < width() * 0.27){
        a = fnt.pixelSize();
        a++;
        if(a > 300)
            break;
        fnt.setPixelSize(a);
        fm = QFontMetrics(fnt);
        lbl_total->setFont(fnt);
    }
    fnt.setPixelSize(a * 0.7);
    lbl_total->setFont(fnt);
    lbl_1->setFont(fnt);
    lbl_2->setFont(fnt);
    lbl_3->setFont(fnt);
    lbl_4->setFont(fnt);
    lbl_5->setFont(fnt);
    lbl_sum->setFont(fnt);
    lbl_priem->setFont(fnt);
}

void FirstRound::paintEvent(QPaintEvent*){
    QPainter qp;
    qp.begin(this);
    int w = width();
    int h = height() - 22;
    int line_1_left = lbl_priem->geometry().left() - 6;
    int line_1_top = lbl_priem->geometry().top() - 6;
    int line_2_top = lbl_total->geometry().top() - 6;
    int line_3_top = lbl_1->geometry().top() - 6;
    QPen pen = QPen(QBrush(QColor("yellow")), 3);
    qp.setPen(pen);
    qp.drawLine(line_1_left - (w - 22) / 72, line_1_top, line_1_left - (w - 22) / 72, h + 11);
    qp.drawLine(0, line_2_top, w, line_2_top);
    qp.drawLine(0, line_3_top, w, line_3_top);
    if(((FormMain*)p)->sett->getRbEng())
        qp.drawLine((w - 22) * 26 / 36 + 11, 0, (w - 22) * 26 / 36 + 11, line_3_top);    //линия между фамилией и регионом
    else
        qp.drawLine(w / 2, 0, w / 2, line_3_top);
    qp.end();
}

void FirstRound::keyPressEvent(QKeyEvent* pe){
    if(pe->key() == Qt::Key_Escape)
        QGuiApplication::quit();
}
