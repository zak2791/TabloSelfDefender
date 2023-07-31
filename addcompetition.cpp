#include "addcompetition.h"
#include "ui_addcompetition.h"
#include <QDebug>

AddCompetition::AddCompetition(bool b, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCompetition)
{
    ui->setupUi(this);
    state = b;
    setWindowTitle("Новое соревнование");
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

AddCompetition::~AddCompetition()
{
    delete ui;

}
