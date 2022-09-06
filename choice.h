#ifndef CHOICE_H
#define CHOICE_H

#include <QtWidgets>
#include <ui_FormChoice.h>


class choice : public QWidget, private Ui::FormChoice {
    Q_OBJECT

public:
    choice(QWidget *parent = nullptr);
    virtual void closeEvent(QCloseEvent*);
    ~choice();
};



#endif // CHOICE_H
