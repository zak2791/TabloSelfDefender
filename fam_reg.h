#pragma once
#include <QLabel>

class Fam : public QLabel {
	Q_OBJECT
	//Q_PROPERTY()
public:
	explicit Fam(QString _cl = "yellow",
				 QString _tx = "text",
				 int _wt = 10,
				 QString fn = "",
				 QWidget *parent = 0);
		

	~Fam();

	//int id = 0;
	int align = 0;

public slots:
	void Text(QString);
	void setViewStyle(int);

	//private slots:
		//

	//public:
	

private:
	virtual void paintEvent(QPaintEvent *);
	int wt;
	//QString fn;
	QString tx;
	QFont fm;
};