#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();

    QAction* addOne;
    QAction* AddFromBuffer;

    QAction* ShowHelp;
    QAction* New;
    //QAction* Open;
    QAction* Edit;
    QStatusBar* status;

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
    QMenu* lastCompetitions;

private slots:
    void choiceCompetitions(void);
    void choiceMats(void);  //выбор ковра из меню

signals:
    void sigChoiceCompetitions(QString);
    void sigChoiceMats(QString);

};

#endif // MAINWIN_H
