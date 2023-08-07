#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();

    QAction* AddOne;
    QAction* AddFromBuffer;

    QAction* AddOneRef;
    QAction* AddRefFromBuffer;

    QAction* ShowHelp;
    QAction* New;
    //QAction* Open;
    QAction* Edit;
    QStatusBar* status;

    QLabel* lblStatus;

    QMenu* lastCompetitions;
    QMenu* Sportsmens;
    QMenu* Referees;

    void fillLastCompetitions(void);

public slots:
    void choiceMat(QString);    //выбор ковра из программы

private:
    Ui::MainWin *ui;
    QString currentActionText;
    QString currentActionMat;
    QAction* actMatA;
    QAction* actMatB;
    QMenu* menu;

private slots:
    void choiceCompetitions(void);
    void choiceMats(void);  //выбор ковра из меню

signals:
    void sigChoiceCompetitions(QString);
    void sigChoiceMats(QString);

};

#endif // MAINWIN_H
