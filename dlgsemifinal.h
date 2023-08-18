#ifndef DLGSEMIFINAL_H
#define DLGSEMIFINAL_H

#include <QDialog>
#include "FormMain.h"

namespace Ui {
class dlgSemiFinal;
}

class dlgSemiFinal : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSemiFinal(QWidget *parent = nullptr);
    ~dlgSemiFinal();

private:
    Ui::dlgSemiFinal *ui;
    virtual void accept();
    virtual void reject();
    void family_set();
    FormMain* fm;
    QString red, blue;

private slots:
    void btn_clicked(void);

};

#endif // DLGSEMIFINAL_H
