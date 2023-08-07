#include "protocol.h"

#include <QHeaderView>

#include <QPrintPreviewDialog>


Protocol::Protocol(QWidget *parent) : QTableWidget(parent){
    setRowCount(35);
    setColumnCount(35);
//    QStringList m_TableHeader;
//    QTableWidgetItem* item1 = new  QTableWidgetItem;
//    item1->setText("1");
//    QTableWidgetItem* item2 = new  QTableWidgetItem;
//    setShowGrid(true);
//    item2->setText("2");
//    m_TableHeader<<"№"<<"Фамилия,\nИмя"<<"Команда"<<"1"<<"2"<<"3"<<"4"<<"5"<<"S1"<<
//                                                    "1"<<"2"<<"3"<<"4"<<"5"<<"S2"<<
//                                                    "1"<<"2"<<"3"<<"4"<<"5"<<"S3"<<
//                                                    "1"<<"2"<<"3"<<"4"<<"5"<<"S4"<<
//                                                    "1"<<"2"<<"3"<<"4"<<"5"<<"S5"<<
//                                                    "Сумма\nбаллов"<<"Место";
//    setHorizontalHeaderLabels(m_TableHeader);
//    setHorizontalHeaderItem(2, item1);



    setSpan(0, 0, 2, 1);
    setSpan(0, 1, 2, 1);
    setSpan(0, 2, 2, 1);
    setSpan(0, 3, 1, 30);
    setSpan(0, 33, 2, 1);
    setSpan(0, 34, 2, 1);
    QTableWidgetItem*  i1 = new QTableWidgetItem;
    i1->setText("№");
    setItem(0, 0, i1);
    QTableWidgetItem*  i2 = new QTableWidgetItem;
    i2->setText("Фамилия,\nИмя");
    setItem(0, 1, i2);
    QTableWidgetItem*  i3 = new QTableWidgetItem;
    i3->setText("Команда");
    setItem(0, 2, i3);
    QTableWidgetItem*  i4 = new QTableWidgetItem;
    i4->setText("Оценки выступления");
    i4->setTextAlignment(Qt::AlignCenter);
    setItem(0, 3, i4);
    QTableWidgetItem*  i5 = new QTableWidgetItem;
    i5->setText("1");
    setItem(1, 3, i5);
    QTableWidgetItem*  i6 = new QTableWidgetItem;
    i6->setText("2");
    setItem(1, 4, i6);
    QTableWidgetItem*  i7 = new QTableWidgetItem;
    i7->setText("3");
    setItem(1, 5, i7);
    QTableWidgetItem*  i8 = new QTableWidgetItem;
    i8->setText("4");
    setItem(1, 6, i8);
    QTableWidgetItem*  i9 = new QTableWidgetItem;
    i9->setText("5");
    setItem(1, 7, i9);
    QTableWidgetItem*  i10 = new QTableWidgetItem;
    i10->setText("S1");
    setItem(1, 8, i10);

    this->verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(false);
    setStyleSheet("QTableView {selection-background-color: red;}");
    setGeometry(100, 100, 1000, 800);
    //setFixedSize(1000, 800);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    show();

    QPixmap pixmap(size());
    render(&pixmap);

    printer = new QPrinter;
    render(printer);
    QPrintPreviewDialog  *prDialog = new QPrintPreviewDialog(&pixmap, this);




    connect(prDialog, SIGNAL(paintRequested()),this,SLOT(printPreviewNeeds()));

    prDialog->showMaximized();
    prDialog->exec();

}

void Protocol::printPreviewNeeds(QPrinter*){
    render(printer);
}
