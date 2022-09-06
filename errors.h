#ifndef ERRORS_H
#define ERRORS_H

#include <ui_FormErrors.h>
#include <QStringListModel>
#include <QItemDelegate>
#include <QPainter>

#include <formcorrection.h>

class MyFilter : public QObject{
    Q_OBJECT

public:
    MyFilter(QLabel*, QWidget*, QWidget* parent=nullptr);

private:

    QLabel* parent_led;                     //label, на который нажали
    QWidget* parent;                         //главный класс приложения
    QWidget* parent_form;                    //форма показа ошибок

public:
    virtual bool eventFilter(QObject*, QEvent*);
};


class MyDelegate : public QItemDelegate{
    Q_OBJECT

public:
      QWidget* p;
      MyDelegate(QWidget* parent=nullptr);

private:
    virtual void paint(QPainter*, const QStyleOptionViewItem &, const QModelIndex &) const;
};

class Errors : public QWidget, private Ui::FormErrors{
    Q_OBJECT

private:
    QStringListModel* mdl;
    MyDelegate* delegate;
    FormCorrection* corr;
    QStringList list_id_rate;
    int row;
    //QListView* LNames;
    virtual void showEvent(QShowEvent*);
private slots:
    void selection(QItemSelection, QItemSelection);

private slots:
    void btnPrint_clicked(void);

public:
    Errors(QWidget* parent = nullptr);
    QWidget* p;
    QList<QString> rates;
    QString sel_data;


    void sh(QLabel*, QWidget*, QWidget*);
    QLabel* LBLTOTALSUM;

};

#endif // ERRORS_H
