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
    QAction* MatA;
    QAction* MatB;
    QAction* ShowHelp;
    QAction* New;
    QAction* Open;
    QAction* OpenLast;

private:
    Ui::MainWin *ui;
};

#endif // MAINWIN_H
