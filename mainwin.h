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
    //QAction* MatA;
    //QAction* MatB;
    QAction* ShowHelp;
    QAction* New;
    //QAction* Open;
    QAction* Edit;
    QStatusBar* status;

private:
    Ui::MainWin *ui;
    QString currentActionText;
    QString currentActionMat;

private slots:
    void choiceCompetitions(void);
    void choiceMats(void);

signals:
    void sigChoiceCompetitions(QString);
    void sigChoiceMats(QString);

};

#endif // MAINWIN_H
