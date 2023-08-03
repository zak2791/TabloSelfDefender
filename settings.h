#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>
#include <QLineEdit>
#include <QDateEdit>

#include <ui_FormSettings.h>
#include "registration.h"

//#include "FormMain.h"

class Settings : public QWidget, private Ui::FormSettings{
    Q_OBJECT

    QStringListModel mdl;
    QString date;

    QString ip;
    QString code;
    QString id;
    boolean regFail;

    QWidget* pW;

    QDate qDate;

    QList<QString> l;

    void writeReferees(void);
    virtual void closeEvent(QCloseEvent* e);
    void flag_set(QLabel*, QString);

    Registration* reg;



public:
    Settings(QString _city,
             QString _name_competition,
             QString _date,
             QWidget *parent = nullptr);

    bool getRbEng(void);

    virtual void showEvent(QShowEvent*);

    QRadioButton* RbnSeparate;
    QRadioButton* RbnTogether;
    QRadioButton* RbEng;
    QLineEdit* LED_CITY;
    QLineEdit* NAME_COMPETITION;
    QDateEdit* DATEEDIT;

signals:
    void setPort(QString);

private slots:
    void registration(void);
    void judge_main(int);
    void judge_secretary(int);
    void judge_1(int);
    void judge_2(int);
    void judge_3(int);
    void judge_4(int);
    void judge_5(int);
    void separate(bool);
    void team_referees(bool);
    void rus_to_eng(bool);
    void showFlag(int);
    void ChangeConnect2(void);
    void registrationReset(void);
    void registrationSuccesfull(QString, QString, QString);
    void registrationFailed(void);
    void registrationStart(QString);
    void ts(int);
};

#endif // SETTINGS_H
