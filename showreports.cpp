#include "showreports.h"
#include "FormMain.h"




ShowReports::ShowReports(int _num_round,
 QWidget* parent) : QWidget(parent){

    p = parent;



    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(((FormMain*)p)->currentDataBase);
    if(!m_db.open()){
        QMessageBox msgBox;
        msgBox.setText("Ошибка базы данных (btn_enterName_clicked)!");
        msgBox.exec();
        return;
    }

    setWindowFlags(Qt::Window);

    setWindowState(Qt::WindowMaximized);

    printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    printer->setOrientation(QPrinter::Landscape);

    //printer->setFullPage(true);

    //printer->setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    dlg = new QPrintDialog(printer);
    QPushButton* button = new QPushButton("print");
//    QWebEngineView webView;
//    webView.load(QUrl("http://www.bhv.ru"));
//    webView.show();



    textEdit = new QTextEdit();
    QTextDocument* doc = new QTextDocument();
    textEdit->setDocument(doc);
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
    sql = "SELECT * "//id, id_sportsmen, place"
          " FROM errors_and_rates WHERE id_round = '%1' ORDER BY place ASC;";

    sql = sql.arg(ID[0]);
    query.prepare(sql);
    query.exec();
    QStringList id_name;
    QStringList id_reffery_rates;
    //QStringList place;
    while(query.next()){
        id_name.append(query.value(1).toString());
        id_reffery_rates.append(query.value(0).toString());     //id круга
    }

    // выбор фамилий спортсменов
    //QStringList name;
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
            "total, place "
            "FROM errors_and_rates WHERE id " + id + " ORDER BY place ASC";

    query.exec(sql);

    while(query.next()){
        QStringList l;
        for(int i=0;i<32;i++)
            l.append(query.value(i).toString());
        rates.append(l);
    }
    sql =   "SELECT "
            "errors1_1, errors2_1, errors3_1, errors4_1, errors5_1,"
            "errors1_2, errors2_2, errors3_2, errors4_2, errors5_2,"
            "errors1_3, errors2_3, errors3_3, errors4_3, errors5_3,"
            "errors1_4, errors2_4, errors3_4, errors4_4, errors5_4,"
            "errors1_5, errors2_5, errors3_5, errors4_5, errors5_5 "
            "FROM errors_and_rates WHERE id " + id + " ORDER BY place ASC";

    query.exec(sql);

    while(query.next()){
        QStringList l;
        for(int i=0;i<25;i++)
            l.append(query.value(i).toString());
        errors.append(l);
    }


    st = static_cast<FormMain*>(p)->currentDataBase.split("_").at(1); //_st;        //"Брянск"

    name_competition = static_cast<FormMain*>(p)->currentDataBase.split("_").at(0); //_name_competition;
    QString ds = static_cast<FormMain*>(p)->currentDataBase.split("_").at(2);       //дата начала соревнований
    QString df = static_cast<FormMain*>(p)->currentDataBase.split("_").at(3);       //дата окончания соревнований
    df.remove(df.length() - 3, 3);
    if(ds == df)
        date = ds;
    else
        date = ds + " - " + df;
    //date = _date;
    __num_round = _num_round;

    QString html = createHTML(0);

    doc->setHtml(html);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(textEdit);
    vbox->addWidget(button);
    setLayout(vbox);
    connect(button, SIGNAL(clicked()), this, SLOT(prt()));
    m_db.close();



    QStringList list;
    for(int i=0; i<name.count(); i++){
        list.append(name[i] + rates[i]);
    }

    //Protocol protocol(list);
    QString s = static_cast<FormMain*>(p)->CmbAge->currentText() + " лет, ";
       s.append(static_cast<FormMain*>(p)->CmbWeight->currentText() + " кг, ");
       s.append(static_cast<FormMain*>(p)->Cmb_round->currentText() + ".html");
    QFile f;
    f.setFileName(s);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);
    out << html;
    f.close();

    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(s)));

}

QString ShowReports::createHTML(int mode){
    //int num_round = __num_round;
    QString html = "";
    //html += "<center>\n";
    html += "<html>\n";


    //if(mode == 1){
        html +=
            "<style>"
                "table.print{"
                    "border: 1px solid black;"
                    "border-collapse: collapse;"
                "}"

                "[data-tooltip] {"
                    "position: relative;" /* Относительное позиционирование */
                "}"
                "[data-tooltip]::after {"
                    "content: attr(data-tooltip);" /* Выводим текст */
                    "position: absolute;" /* Абсолютное позиционирование */
                    "width: 100px;" /* Ширина подсказки */
                    "left: 0; top: 0;" /* Положение подсказки */
                    "background: #3989c9;" /* Синий цвет фона */
                    "color: #fff;" /* Цвет текста */
                    "padding: 0.5em;" /* Поля вокруг текста */
                    "box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);" /* Параметры тени */
                    "pointer-events: none;" /* Подсказка */
                    "opacity: 0;" /* Подсказка невидима */
                    "transition: 0.5s;" /* Время появления подсказки */
                "}"
                "[data-tooltip]:hover::after {"
                    "opacity: 1;" /* Показываем подсказку */
                    "top: 2em;" /* Положение подсказки */
                "}"
            "</style>";

    //}
    html += "<body><big>\n";

    //html += "<p align=\"center\">П Р О Т О К О Л</p><br>\n";

    html += "<table style=\"border-width: 0px;\" align=\"center\" width=\"100%\">\n";

    html += "<tr>\n";
    html += "<td align=\"center\" colspan=\"3\">\n";
    html += "<b>П Р О Т О К О Л</b>\n";
    html += "</td>\n";

    html += "<tr>\n";
    html += "<td align=\"center\" colspan=\"3\">\n";
    html += name_competition;
    html += "</td>\n";
    html += "</tr>\n";

    html += "<tr>\n";
    html += "<td width=\"20%\" align=\"left\">\n";

    html += date + " г.";
    html += "</td>\n";

    html += "<td width=\"60%\" align=\"center\">\n";
    html += "возраст " + static_cast<FormMain*>(p)->CmbAge->currentText() +
                " лет, вес " + static_cast<FormMain*>(p)->CmbWeight->currentText() + " кг ";
    html += static_cast<FormMain*>(p)->Cmb_round->currentText();
    html += "</td>\n";

    html += "<td width=\"20%\" align=\"right\">\n";
    html += "гор.";
    html += "&nbsp;";
    html += "&nbsp;";
    html += st;
    html += "</td>\n";
    html += "</tr>\n";

    html += "</table>\n";
/*---------------------------------------------------------------------------------*/
    html += "<br>";
    //if(mode == 0){
    //    html += "<table border "//style=\"vertical-align: middle; border-width: 1px; border-style: solid; border-color: #000000;\" "
    //            "width=\"100%\" cellspacing = \"0\" cellpadding = \"2\"   margin=\"auto\" >\n";
    //}else
        html += "<table rules=\"all\" class=\"print\" width=\"100%\" margin=\"auto\">\n";

    html += "<tr>\n";
    html += "<td  width=\"2%\" rowspan=\"2\"><font size=\"2\">\n";
    html += "№ </font><br>п/п\n";
    html += "</td>\n";
    html += "<td width=\"15%\" align=\"center\" rowspan=\"2\">Фамилия,<br>имя</td>\n";
    html += "<td  align=\"center\" width=\"15%\" rowspan=\"2\">Команда,<br>разряд</td>\n";

    html += "<td align=\"center\" width=\"60%\" colspan=\"30\">Оценки выступления</td>\n";

    html += "<td align=\"center\"  width=\"5%\" rowspan=\"2\"><font size=\"2\">Сумма<br>баллов</font></td>\n";
    html += "<td align=\"center\" width=\"3%\" rowspan=\"2\"><font size=\"2\">Место</font></td>\n";
    html += "</tr>\n";
    html += "<tr>\n";
    int i=1;

    int count_judge = 6;

    // печать шапки таблицы
    int j;
    while(i < 6){
        j = 1;
        while(j < count_judge){
            html += "<td align=\"center\"><font size=\"2\">"+QString::number(j)+"</font></td>\n";
            j++;
        }
        html += "<td align=\"center\"><font size=\"2\">S"+QString::number(i)+"</font></td>\n";
        i++;
    }
    html += "</tr>\n";
    int count = 0;
    int count_color = 0;
    QString color = "White";

    while(count < name.length()){
        count++;
        if(static_cast<FormMain*>(p)->flag_mode == 1){
            count_color++;
            if(count_color < 3)
                color = "LightGray";
            else
                color = "White";
            if(count_color == 4)
                count_color = 0;
        }
        html += "<tr bgcolor=" + color  + " align=\"center\"><td align=\"center\">" + QString::number(count) + "</td>\n";    // порядковый номер
        html += "<td align=\"left\"><font size=\"3\">" + name[count - 1][0] + "</font></td>\n";                                                    // фамилия, имя
        html += "<td align=\"left\"><font size=\"2\">" + name[count - 1][1] + "</font></td>\n";                                   // регион

        for(int i=0;i<32;i++){
            if(rates[count - 1][i] != NULL){
                if(rates[count - 1][i][0] == "N"){
                    html += "<td " + tooltip(count, i) + " align=\"center\" width=\"2%\"><font size=\"2\"><s>\n" +
                            rates[count - 1][i].remove(0, 1) + "</s></font></td>";
                }else{
                    if(i == 30){
                        html += "<td align=\"center\" width=\"2%\"><font size=\"3\">" +
                                rates[count - 1][i] + "</font></td>\n";
                    }else{
                        html += "<td " + tooltip(count, i) + " align=\"center\" width=\"2%\"><font size=\"2\">" +
                                rates[count - 1][i] + "</font></td>\n";
                    }
                }
            }else
                html = html + "<td align=\"center\" width=\"2%\"><font size=\"2\">" + "" + "</font></td>\n";
        }
        //html = html + "<td align=\"center\" width=\"2%\"><font size=\"2\">" + "" + "</font></td>\n";
        html += "</tr>\n";
    }
    html += "</tr>\n";
    html += "</table>\n";
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    html += "<br>";

    html += "<table style=\"border-width: 0px;\" align=\"center\" width=\"100%\" >\n";

    html += "<tr>\n";

    html += "<td width=\"20%\" >\n";
    html += "Главный судья";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td  width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->main_refery.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->main_refery.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "<td width=\"10%\" >\n";
    html += "</td>\n";

    html += "<td width=\"20%\" >\n";
    html += "Главный секретарь";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->main_secretary.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->main_secretary.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "</tr>\n";

    html += "<tr>\n";
    html += "</tr>\n";

    html += "<tr>\n";

    html += "<td width=\"20%\" >\n";
    html += "Руководитель ковра";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td  width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->judge1.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->judge1.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "<td width=\"10%\" >\n";
    html += "</td>\n";

    html += "<td width=\"20%\" >\n";
    html += "Судья 2";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->judge2.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->judge2.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "</tr>\n";

    html += "<tr>\n";
    html += "</tr>\n";

    html += "<tr>\n";

    html += "<td width=\"20%\" >\n";
    html += "Судья 3";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>";

    html += "<td  width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->judge3.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->judge3.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "<td width=\"10%\" >\n";
    html += "</td>\n";

    html += "<td width=\"20%\" >\n";
    html += "Судья 4";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->judge4.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->judge4.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "</tr>\n";

    html += "<tr>\n";
    html += "</tr>\n";

    html += "<tr>\n";

    html += "<td width=\"20%\" >\n";
    html += "Судья 5";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td\n>";

    html += "<td  width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->judge5.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->judge5.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "<td width=\"10%\" >\n";
    html += "</td>\n";

    html += "<td width=\"20%\" >\n";
    html += "Технический секретарь";
    html += "</td>\n";

    html += "<td align=\"right\" width=\"5%\" >\n";
    //html += "______________";
    html += "</td>\n";

    html += "<td width=\"20%\" ><u><em>\n";
    html += static_cast<FormMain*>(p)->ts.split(';')[0];
    //for(int i=0;i<(25 - static_cast<FormMain*>(p)->ts.split(';')[0].trimmed().length());i++)
    //    html += "&nbsp;";
    html += "</u></em></td>\n";

    html += "</tr>\n";

    html += "</table>\n";

    html += "</body></big></html>\n";
    //html += "</center>";

    return html;
}

QString ShowReports::tooltip(int count, int i){
    QString err;
    if(i < 5)
        err = errors[count - 1][i];
    else if((i > 5) && (i < 11))
        err = errors[count - 1][i - 1];
    else if((i > 11) and (i < 17))
        err = errors[count - 1][i - 2];
    else if((i > 17) and (i < 23))
        err = errors[count - 1][i - 3];
    else if((i > 23) and (i < 29))
        err = errors[count - 1][i - 4];
    else
        return "";


    if(err.length() > 2){
        QString s = "";
        foreach(QString each, err){
            if((each != '\'') && (each != '[') && (each != ']'))
                s = s + each;
        }
        return "data-tooltip=\"" + s + "\"";
    }else
        return "";
}

void ShowReports::prt(void){
    if(dlg->exec() == QDialog::Accepted){
        QString s = static_cast<FormMain*>(p)->CmbAge->currentText() + " лет, ";
           s.append(static_cast<FormMain*>(p)->CmbWeight->currentText() + " кг, ");
           s.append(static_cast<FormMain*>(p)->Cmb_round->currentText() + ".html");
        QFile f;
        f.setFileName(s);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&f);
        out << createHTML(1);
        f.close();
        textEdit->print(printer);
    }
}
