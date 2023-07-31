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
    explicit AddCompetition(QWidget *parent = nullptr);
    ~AddCompetition();

private:
    Ui::AddCompetition *ui;
};

#endif // ADDCOMPETITION_H
