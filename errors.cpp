#include "errors.h"
#include "FormMain.h"

#include <QSqlQuery>

MyFilter::MyFilter(QLabel* _parent_led, QWidget* _parent_form, QWidget* _parent):QObject(_parent){

    parent_led = _parent_led;        // label, на который нажали
    parent = _parent;                // главный класс приложения
    parent_form = _parent_form;      // форма показа ошибок

}

bool MyFilter::eventFilter(QObject*, QEvent* e){
    if(e->type() == QEvent::MouseButtonPress){
        //self.corr.sh(self.p)
        //self.parent.err.sh()
        //показ формы ввода оценок
        static_cast<Errors*>(parent_form)->sh(parent_led, parent_form, parent);

        return true;
    }
    return false;
};

MyDelegate::MyDelegate(QWidget* parent):QItemDelegate(parent){
    p = parent;
};



void MyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const{
    if(((FormMain*)p)->flag_mode == 1){
        //set background color
        int num = index.row();
        while(num >= 0)
            num -= 4;
        num += 4;
        painter->setPen(QPen(Qt::NoPen));
        if(num == 0 or num == 1)
            painter->setBrush(QBrush(Qt::lightGray));
        else
            painter->setBrush(QBrush(Qt::white));
        painter->drawRect(option.rect);
        //set text color
        painter->setPen(QPen(Qt::black));
    }
    QItemDelegate::paint(painter, option, index);
};



Errors::Errors(QWidget* parent):QWidget(){
    setupUi(this);

    p = parent;
    delegate = new MyDelegate(p);
    sel_data = QString("");
    LBLTOTALSUM = lblTotalSum;
    mdl = new QStringListModel();
    //LNames = lNames;
    lNames->setModel(mdl);
    lNames->setItemDelegate(delegate);
    connect(lNames->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this,
            SLOT(selection(QItemSelection, QItemSelection)));
    connect(btnPrint, SIGNAL(clicked()), this, SLOT(btnPrint_clicked()));
    for(int i=1;i<6;i++){
        for(int j=1;j<6;j++){
            QLabel* led = findChild<QLabel*>("leErr" + QString::number(i) + QString::number(j));
            led->installEventFilter(new MyFilter(led, this, p));
        }
    }
};

void Errors::showEvent(QShowEvent*){
    ////////////////////////////////////////////////////////////////////////////////////
    //           Получение списка спортсменов из таблицы errors_and_rates             //
    ////////////////////////////////////////////////////////////////////////////////////
    QString sql = "SELECT id, id_sportsmen FROM errors_and_rates WHERE id_round = '%1' ORDER BY id";
    int id_round = static_cast<FormMain*>(p)->id_round;
    sql = sql.arg(id_round);
    QSqlQuery query;
    QList<QStringList> athlets;
    if(!query.exec(sql))
        qDebug() << "errors.cpp 1";
    else{
        while(query.next()){
            QStringList sl;
            sl.append(query.value(0).toString());
            sl.append(query.value(1).toString());
            athlets.append(sl);
        }
    }
    list_id_rate.clear();
    QStringList id_name;
    foreach(QStringList each, athlets){
        list_id_rate.append(each[0]);
        //выбор фамилий спортсменов
        id_name.append(each[1]);
    }
    QList<QStringList> name;
    foreach(QString i, id_name){
        sql = "SELECT name, region FROM sportsmens WHERE id = " + i;
        if(!query.exec(sql))
            qDebug() << "errors.cpp 2";
        while(query.next()){
            QStringList sl;
            sl.append(query.value(0).toString());
            sl.append(query.value(1).toString());
            name.append(sl);
        }

    }
    QStringList l;
    foreach(QStringList each, name){
        l.append(each[0] + ", " + each[1]);
    }
    mdl->setStringList(l);
    for(int i=1;i<6;i++){
        QLabel* led = findChild<QLabel*>("lblSum" + QString::number(i));
        led->setText("");
        for(int j=1;j<6;j++){
            QLabel* led = findChild<QLabel*>("lbRate" + QString::number(i) + QString::number(j));
            led->setText("");
            led = findChild<QLabel*>("leErr" + QString::number(i) + QString::number(j));
            led->setText("");
        }
    }
    lblTotalSum->setText("");
}

///////////////////////////////////////////////////////////////////////////////
//                       выбор спортсмена в списке                           //
///////////////////////////////////////////////////////////////////////////////
void Errors::selection(QItemSelection a, QItemSelection){
    sel_data = a.indexes()[0].data().toString();           // фамилия, регион
    row = a.indexes()[0].row();                 // строка
    // выбор оценок
    QString sql =   "SELECT "
                    "errors1_1, errors2_1, errors3_1, errors4_1, errors5_1, "
                    "errors1_2, errors2_2, errors3_2, errors4_2, errors5_2, "
                    "errors1_3, errors2_3, errors3_3, errors4_3, errors5_3, "
                    "errors1_4, errors2_4, errors3_4, errors4_4, errors5_4, "
                    "errors1_5, errors2_5, errors3_5, errors4_5, errors5_5, "
                    "rate1_1,   rate2_1,   rate3_1,   rate4_1,   rate5_1, "
                    "rate1_2,   rate2_2,   rate3_2,   rate4_2,   rate5_2, "
                    "rate1_3,   rate2_3,   rate3_3,   rate4_3,   rate5_3, "
                    "rate1_4,   rate2_4,   rate3_4,   rate4_4,   rate5_4, "
                    "rate1_5,   rate2_5,   rate3_5,   rate4_5,   rate5_5, "
                    "sum1,      sum2,      sum3,      sum4,      sum5, "
                    "total "
                    "FROM errors_and_rates WHERE id = " + list_id_rate[row];
    QSqlQuery query;
    if(!query.exec(sql))
        qDebug() << "errors.cpp 3";

    rates.clear();
    query.next();
    for(int i=0; i<56;i++)
        rates.append(query.value(i).toString());

    int count = 0;
    // вывод ошибок
    for(int i=1;i<6;i++){
        for(int j=1;j<6;j++){
            QLabel* led = findChild<QLabel*>("leErr" + QString::number(i) + QString::number(j));
            QString _data = rates[count];
            count++;
            if(_data != NULL){
                QRegExp reg("[\\d+]+");
                QStringList list;
                int pos = 0;
                while ((pos = reg.indexIn(_data, pos)) != -1) {
                    list << reg.cap(0);
                    pos += reg.matchedLength();
                }
                QString t = "";
                try{
                    foreach(QString each, list)
                        t = t + ", " + each;
                    t = t.remove(0, 2);
                }catch(...){
                    qDebug() << "error re";
                }
                led->setText(t);
            }else
                led->setText("");
            }
        }
    // вывод баллов
    QString _data;
    for(int i=1;i<6;i++){
        for(int j=1;j<6;j++){
            QLabel* led = findChild<QLabel*>("lbRate" + QString::number(i) + QString::number(j));
            _data = rates[count];
            count++;
            if(_data != NULL){
                QString d;
                if(_data[0] == 'N'){
                    d = "<s>";
                    d += _data.remove(0, 1) + "</s>";
                    led->setText(d);
                }else
                    led->setText(_data);
            }else
                led->setText("");
        }
    }
    for(int i=1;i<6;i++){
        QLabel* led = findChild<QLabel*>("lblSum" + QString::number(i));
        _data = rates[count];
        count++;
        if(_data != NULL)
            led->setText(_data);
        else
            led->setText("");
    }
    _data = rates[55];
    if(_data != NULL)
        lblTotalSum->setText(_data);
    else
        lblTotalSum->setText("");
}

///////////////////////////////////////////////////////////////////////////////////
//                  вызов формы с кнопками корекции результата                   //
///////////////////////////////////////////////////////////////////////////////////
void Errors::sh(QLabel* a, QWidget* b, QWidget* c){
    int question = QString(a->objectName()[5]).toInt();         // выбранный вопрос
    QString id_rate = list_id_rate[row];                        // выбранная запись
    FormMain* f;
    f = static_cast<FormMain*>(c);
    if(f->enable_correct == true){
        if(f->flag_mode == 0){
            if(f->id_rates == id_rate.toInt()){
                if(f->flag_priem == question){
                    a->setStyleSheet("background-color:#ff0000;");
                    corr = new FormCorrection(a, b, c);
                    corr->show();
                }
            }
        }else{
            if(f->id_correct == id_rate.toInt()){
                if (int((float)(f->flag_priem) / 2.0 + 0.5) == question){
                    a->setStyleSheet("background-color:#ff0000;");
                    corr = new FormCorrection(a, b, c);
                    corr->show();
                }
            }
        }
    }
}

void Errors::btnPrint_clicked(void){
    QTextBrowser* te = new QTextBrowser();
    QString html = "<h3 align=center>" + sel_data + ", " +
                    static_cast<FormMain*>(p)->CmbAge->currentText()  +  " лет, " +
                    static_cast<FormMain*>(p)->CmbWeight->currentText() +  " кг, " +
                    static_cast<FormMain*>(p)->Cmb_round->currentText() + "</h3><br>";
    html += "<table border=\"1\" width=\"100%\" cellpadding=\"5\" align=center valign=middle>"
                "<tr>"
                    "<td width=\"10%\" align=\"center\">Вопросы</td>"
                    "<td width=\"10%\" align=\"center\">Судьи</td>"
                    "<td width=\"25%\" align=\"center\">Ошибки</td>"
                    "<td width=\"15%\" align=\"center\">Оценка</td>"
                    "<td width=\"15%\" align=\"center\">Сумма</td>"
                    "<td width=\"15%\" align=\"center\">Общая сумма</td>"
                "</tr>";
    for(int i=1;i<6;i++){
    QLabel* lblSum = findChild<QLabel*>("lblSum" + QString::number(i));
    html += "<tr>"
                "<td rowspan=\"5\" align=center>" + QString::number(i) + "</td>";

        for(int j=1;j<6;j++){
            QLabel* lblErr = findChild<QLabel*>("leErr" + QString::number(i) + QString::number(j));
            QLabel* lblRate = findChild<QLabel*>("lbRate" + QString::number(i) + QString::number(j));
            if(j != 1)
                html += "<tr>";
            html += "<td align=center>" + QString::number(j) + "</td>" +
                    "<td>" + lblErr->text() + "</td>" +
                    "<td align=center>" + lblRate->text() + "</td>";
            if(j != 1)
                html += "</tr>";
            if(j == 1){
                if(i == 1){
                    html += "<td rowspan=\"5\" align=center>" + lblSum->text() + "</td>" +
                            "<td rowspan=\"25\" align=center>" + lblTotalSum->text() + "</td>" +
                            "</tr>";
                }else{
                    html += "<td rowspan=\"5\" align=center>" + lblSum->text() + "</td>" +
                            "</tr>";
                }
            }
        }
    }
    html += "</table>";
    te->setText(html);
    QPrinter* printer = new QPrinter();
    QPrintDialog dlg(printer);
    if(dlg.exec() == QDialog::Accepted)
        te->print(printer);
}

