#pragma once
#include <QLabel>
#include <QTimer>

class Ball : public QLabel {
	Q_OBJECT
public:
    explicit Ball(int _diff = 1,
                  QString style = "style_red.qss",
                  QWidget *parent = nullptr);

	~Ball();

	int id = 0;

	void plus();
	void minus();
    void sbros(void);
    void setRate(QString);

signals:
	void ball(int);

private slots:
    void on_timeout(void);


private:
    float rt;
    int diff;
    QTimer* timer;
    float opacity;
    int count_opacity;

    virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);





};
