#include "protocol.h"


#include <QHeaderView>

#include <QStandardItemModel>
#include <QDebug>
#include <QSqlDatabase>

Protocol::Protocol(FormMain* parent) : QTableView(){
    p = parent;

    setSortingEnabled(true);
}

void Protocol::updateProtocol(){
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(((FormMain*)p)->currentDataBase);
    if(!m_db.open()){
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (btn_enterName_clicked)!");
        msgBox.exec();
        return;
    }

    // выбор текущего раунда
    QString sql = "SELECT id FROM rounds WHERE round = '%1' and age = '%2' and weight = '%3'";
    sql = sql.arg(static_cast<FormMain*>(p)->Cmb_round->currentIndex() + 1)
             .arg(static_cast<FormMain*>(p)->CmbAge->currentText())
             .arg(static_cast<FormMain*>(p)->CmbWeight->currentText());
    QSqlQuery query;
    query.exec(sql);
    QStringList ID;
    if(!query.next()){
        m_db.close();
        return;
    }
    query.previous();
    while(query.next()){
        ID.append(query.value(0).toString());
    }
    // выбор спортсменов и записей оценок из таблицы ошибок
    sql = "SELECT id, id_sportsmen FROM errors_and_rates WHERE id_round = '%1' ORDER BY id";
    sql = sql.arg(ID[0]);
    query.exec(sql);
    QStringList id_name;
    QStringList id_reffery_rates;
    while(query.next()){
        id_name.append(query.value(1).toString());
        id_reffery_rates.append(query.value(0).toString());
    }

    // выбор фамилий спортсменов
    QList<QStringList> name;
    foreach(QString i, id_name){
        sql = "SELECT name, region FROM sportsmens WHERE id = " + i;
        query.exec(sql);
        QStringList l;
        query.next();
        l.append(query.value(0).toString());
        l.append(query.value(1).toString());
        name.append(l);
    }

    // выбор оценок
    QString id;
    if(id_reffery_rates.length() == 1)
        id = "= " + id_reffery_rates[0];
    else
        id = " IN ( " + id_reffery_rates.join(", ") + " )";
    sql =   "SELECT "
            "rate1_1, rate2_1, rate3_1, rate4_1, rate5_1, sum1, "
            "rate1_2, rate2_2, rate3_2, rate4_2, rate5_2, sum2, "
            "rate1_3, rate2_3, rate3_3, rate4_3, rate5_3, sum3, "
            "rate1_4, rate2_4, rate3_4, rate4_4, rate5_4, sum4, "
            "rate1_5, rate2_5, rate3_5, rate4_5, rate5_5, sum5, "
            "total, place, id "
            "FROM errors_and_rates WHERE id " + id;

    query.exec(sql);

    QList<QStringList> rates;
    while(query.next()){
        QStringList l;
        for(int i=0;i<33;i++)
            l.append(query.value(i).toString());
        rates.append(l);
    }
    sql =   "SELECT "
            "errors1_1, errors2_1, errors3_1, errors4_1, errors5_1,"
            "errors1_2, errors2_2, errors3_2, errors4_2, errors5_2,"
            "errors1_3, errors2_3, errors3_3, errors4_3, errors5_3,"
            "errors1_4, errors2_4, errors3_4, errors4_4, errors5_4,"
            "errors1_5, errors2_5, errors3_5, errors4_5, errors5_5 "
            "FROM errors_and_rates WHERE id " + id;

    query.exec(sql);

    QList<QStringList> errors;
    while(query.next()){
        QStringList l;
        for(int i=0;i<25;i++)
            l.append(query.value(i).toString());
        errors.append(l);
    }

   //__num_round = _num_round;

    m_db.close();



    QList<QStringList> list;
    for(int i=0; i<name.count(); i++){
        list.append(name[i] + rates[i]);
    }


    QStandardItemModel* model = new QStandardItemModel(list.count(), 34);
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            //QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
            QStandardItem *item = new QStandardItem(list[row][column]);
            if(column != 33){
                item->setEditable(false);
                model->setItem(row, column, item);
            }
            else{
                item->setEditable(true);
                item->setData(list[row][column + 1]);
                model->setItem(row, column, item);

            }
        }
    }
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(placeChanged(QStandardItem*)));
    model->setHeaderData(0, Qt::Horizontal, "Фамилия Имя");
    model->setHeaderData(1 , Qt::Horizontal, "Команда");
    int j = 0;
    QString s;
    int count = 2;
    for(int i=2; i<33; i++){
        if(++j == 6){
            s = ("S") + QString::number((i - count++) / 5);
            j = 0;
        }
        else
            s = QString::number(j);
        model->setHeaderData(i, Qt::Horizontal, s);
    }
    model->setHeaderData(32, Qt::Horizontal, "Сумма баллов");
    model->setHeaderData(33, Qt::Horizontal, "Место");

    setModel(model);
    setShowGrid(true);
    for (int i = 2; i < 32; ++i) {
        setColumnWidth(i, 10);
    }
    resizeColumnToContents(0);
    resizeColumnToContents(1);

    qDebug()<<list<<this->model()->rowCount()<<this->model()->columnCount();
    sortByColumn(32, Qt::DescendingOrder);
    show();
}

void Protocol::placeChanged(QStandardItem * item){
    qDebug()<<item->row()<<item->data().toInt()<<item->text();
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(((FormMain*)p)->currentDataBase);
    if(!m_db.open()){
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (btn_enterName_clicked)!");
        msgBox.exec();
        return;
    }
    QString sql = "UPDATE errors_and_rates SET place = " + item->text() + " WHERE id = " + QString::number(item->data().toInt());
    QSqlQuery query;
    if(query.exec(sql))
        qDebug()<<"ok placeChanged!";
    else
        qDebug()<<sql<<"error placeChanged!";
}
