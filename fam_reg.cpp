﻿#include <QtCore>
#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>
#include "fam_reg.h"

Fam::Fam(QString _cl,
		 QString _tx,
		 int _wt,
		 QString fn,
		 QWidget *parent) :

	QLabel(parent)
{
	tx = _tx;
    //QPalette palW = palette();
    //palW.setColor(QPalette::WindowText, QColor(_cl));
    //palW.setColor(QPalette::Window, QColor("black"));
    //setPalette(palW);
	setStyleSheet("QLabel{background-color: black; color: " + _cl + "; }");
	setAutoFillBackground(true);
	wt = _wt;  //жирность шрифта
	
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    if (fn != "")
		fm = QFont();
	else
		fm = QFont(fn);
	align = 0;						//0 - центр
									//1 - левый край
									//2 - правый край
};

Fam::~Fam()
{
}

void Fam::setViewStyle(int s){
	if (s == 0)
		setStyleSheet("QLabel{background-color: black; color: white; }");
	else if(s == 1)
		setStyleSheet("QLabel{background-color: black; color: red; }");
	else
		setStyleSheet("QLabel{background-color: black; color: blue; }");
}

void Fam::Text(QString t) {
	tx = t;
	repaint();
}

void Fam::paintEvent(QPaintEvent * pe){
	fm.setPixelSize(height() / 0.9);
	fm.setWeight(wt); //63
	setFont(fm);

	QPainter pn;
	pn.begin(this);

	QFontMetrics p(font()); // .width(tx);

    if(p.horizontalAdvance(tx) >= width())
		pn.drawText(0, height() * 0.9, tx);
	else
		if(align == 0)
            pn.drawText((width() - p.horizontalAdvance(tx)) / 2, height() * 0.9, tx);
		else if(align == 1)
			pn.drawText(0, height() * 0.9, tx);
		else
            pn.drawText(width() - p.horizontalAdvance(tx), height() * 0.9, tx);
    if (align == 0 || align == 1 || p.horizontalAdvance(tx) >= width()) {
		QLinearGradient gr(width() * 0.85, 0, width(), 0);
		gr.setColorAt(0, QColor(0, 0, 0, 0));
		gr.setColorAt(1, QColor(0, 0, 0, 255));
		pn.setBrush(gr);
	}
	pn.setPen(Qt::NoPen);
	pn.drawRect(0, 0, width(), height());

	pn.end();

}

