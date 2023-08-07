#include "choice_one_athlete.h"

Choice_one_athlete::Choice_one_athlete(QStringList sl, FormMain* parent) : QDialog(parent){
    setFixedSize(800,600);
    p = parent;                 // родительский widget
    col = 6;                    // количество колонок в таблице
    //фамилия
    lbl_fam = new QLabel("<font color=\"Black\">Фамилия</font>", this);
    //ввод фамилии
    inFam = new QLineEdit(this);
    inFam->setMaxLength(7);
    l.append("-");
    QFont f;
    f.setPointSize(10);
    inFam->setFont(f);

    tbl = new QTableView();     // таблица

    try{
        foreach(QString each, sl)
            l.append(each);
    }catch(...){
        qDebug() << "err each";
    }

    l.sort();
    int num_fam = l.length();       //количество фамилий в списке
    int raw = num_fam / col;
    if(num_fam % col)
        raw += 1;                   //количество строк в таблице

    QStandardItemModel* mdl = new QStandardItemModel(raw, col);
    int i = 0;
    for(int r=0;r<raw;r++){
        for(int c=0;c<col;c++){
            i++;
            if(i > num_fam)
                break;
             QStandardItem* item = new QStandardItem(l[i-1]);
             mdl->setItem(r, c, item);
        }
    }
    tbl->setModel(mdl);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(lbl_fam, 2);
    hbox->addWidget(inFam, 2);

    QGroupBox* box = new QGroupBox();
    box->setLayout(hbox);

    QVBoxLayout* mainbox = new QVBoxLayout();

    QItemSelectionModel* sel = tbl->selectionModel();
    tbl->setGridStyle(Qt::DashLine);

    tbl->resizeRowsToContents();
    tbl->horizontalHeader()->hide();
    tbl->verticalHeader()->hide();
    tbl->setShowGrid(true);
    tbl->setSelectionMode(QAbstractItemView::SingleSelection);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainbox->addWidget(box);
    mainbox->addWidget(tbl);

    setLayout(mainbox);

    connect(sel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(sel(QItemSelection, QItemSelection)));
    connect(inFam, SIGNAL(textEdited(const QString&)), SLOT(textEdited(QString)));

    sel_data = "-";
    b = "";
    r = "";
    setModal(true);
    show();
}

void Choice_one_athlete::showEvent(QShowEvent*){
    inFam->setFocus();
}

void Choice_one_athlete::textEdited(QString x){
    QStringList inL;
    foreach(QString i, l){
        if(i.toUpper().indexOf(x.toUpper()) == 0)
            inL.append(i);
    }
    QStringList lst;
    if(x == "")
        lst = l;
    else
        lst=inL;

    int num_fam = lst.length();     //количество фамилий в списке
    int raw = num_fam / col;
    if(num_fam % col)
        raw++;                      //количество строк в таблице

    QStandardItemModel* mdl = new QStandardItemModel(raw, col);

    int i = 0;

    for(int c=0;c<col;c++){
        for(int _r=0;_r<raw;_r++){
            i++;
            if(i > num_fam)
                break;
            QStandardItem* item = new QStandardItem(lst[i-1]);
            mdl->setItem(_r, c, item);
        }
    }
    tbl->setModel(mdl);
    connect(tbl->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(sel(QItemSelection, QItemSelection)));
}

void Choice_one_athlete::closeEvent(QCloseEvent*){
    p->fr->rate_total->setText("");
    p->fr->rate_sum->setText("");
    p->fr->count_priem->setText("");

    p->fr->rate_1->setText("");
    p->fr->rate_2->setText("");
    p->fr->rate_3->setText("");
    p->fr->rate_4->setText("");
    p->fr->rate_5->setText("");

    p->fr->rate_1->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                 "border-style: solid; border-color:white;  background-color: black;}");
    p->fr->rate_2->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                 "border-style: solid; border-color:white;  background-color: black;}");
    p->fr->rate_3->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                 "border-style: solid; border-color:white;  background-color: black;}");
    p->fr->rate_4->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                 "border-style: solid; border-color:white;  background-color: black;}");
    p->fr->rate_5->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                 "border-style: solid; border-color:white;  background-color: black;}");

    family_set();

    if((sel_data != "-") && (sel_data != NULL)){
        p->Btn_next->setEnabled(true);
        p->GroupBox->setEnabled(false);
        p->Cmb_round->setEnabled(false);
        p->CmbAge->setEnabled(false);
        p->CmbWeight->setEnabled(false);
        p->Btn_new->setEnabled(false);
        p->BtnNewFinal->setEnabled(false);
        p->mainwin->menuBar()->setEnabled(false);
    }else{
        p->Btn_next->setEnabled(false);
        p->Cmb_round->setEnabled(true);
        p->CmbAge->setEnabled(true);
        p->CmbWeight->setEnabled(true);
        p->Btn_new->setEnabled(true);
        p->BtnNewFinal->setEnabled(true);
        p->mainwin->menuBar()->setEnabled(true);
        if(p->current_mode == -1){
            p->GroupBox->setEnabled(true);
        }
    }
}

void Choice_one_athlete::sel(QItemSelection a, QItemSelection){
    sel_data = a.indexes()[0].data().toString();
    if(sel_data != NULL)
        lbl_fam->setText("<font color=\"black\"size=5>" + sel_data + "</font>");

}

void Choice_one_athlete::resizeEvent(QResizeEvent*){
        for(int i=0;i<col;i++)
            tbl->setColumnWidth(i,(tbl->rect().width()) / col);
}

QString Choice_one_athlete::sep(QString fam){
    QStringList f = fam.split(" ");
    QString fm = "";
    bool fl;
        foreach(QString i , f){
            fl =true;
            foreach(QChar j, i){
                if(!j.isUpper()){
                    if((j != '\'') and (j != '-'))
                        fl = false;
                }
            }
            if(fl)
                fm = fm + " " + i;
        }
        return fm;
}

void Choice_one_athlete::family_set(void){
    p->Lbl_name->setText(sel_data);
    if((sel_data != "-") && (sel_data != NULL)){
        QStringList stt=sel_data.split(';');
        QStringList stt2=stt[0].split(" ");
        p->fr->name->Text(sep(stt2[0].toUpper() + " " + stt2[1]));
        p->fr->region->Text(stt[1]);
        QString path = "flags/";
        path = path + stt[1] + ".png";
        QFile file(path);
        if(file.exists()){
            p->flag.load(path);
            p->fr->lbl_flag->setPixmap(p->flag.scaled(p->fr->lbl_flag->size()));

        }else
            p->fr->lbl_flag->clear();

    }else{
        p->fr->name->Text("");
        p->fr->region->Text("");
        p->fr->lbl_flag->clear();
    }
}
