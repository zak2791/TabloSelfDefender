#include "mainwin.h"
#include "ui_mainwin.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    addOne          = ui->AddOne;
    AddFromBuffer   = ui->AddFromBuffer;
    MatA            = ui->MatA;
    MatB            = ui->MatB;
    ShowHelp        = ui->ShowHelp;
    New             = ui->New;
    Open            = ui->Open;
    OpenLast        = ui->OpenLast;
}

MainWin::~MainWin()
{
    delete ui;
}
