#ifndef ADDCOMPETITION_H
#define ADDCOMPETITION_H

#include <QDialog>

namespace Ui {
class AddCompetition;
}

class AddCompetition : public QDialog
{
    Q_OBJECT

public:
    explicit AddCompetition(bool, QWidget *parent = nullptr);
    ~AddCompetition();

private:
    Ui::AddCompetition *ui;
    bool state; //true - создать, false - редактировать
};

#endif // ADDCOMPETITION_H
