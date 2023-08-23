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
    explicit dlgSemiFinal(QWidget *parent = nullptr, int mode = 2);
    ~dlgSemiFinal();

private:
    Ui::dlgSemiFinal *ui;
    void family_set();
    FormMain* fm;
    QString red, blue;
    QLabel* LblOne;
    QLabel* LblTwo;
    QRadioButton* RbOne;
    QRadioButton* RbTwo;

private slots:
    void btn_clicked(void);
    void btnOk_clicked(void);
    void btnCancel_clicked(void);

};

#endif // DLGSEMIFINAL_H
