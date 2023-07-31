#include "addcompetition.h"
#include "ui_addcompetition.h"
#include <QDebug>

AddCompetition::AddCompetition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCompetition)
{
    ui->setupUi(this);
    setWindowTitle("Новое соревнование");
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

AddCompetition::~AddCompetition()
{
    qDebug()<<"delete";
    delete ui;

}
