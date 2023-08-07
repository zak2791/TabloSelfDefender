#include "choice_two_athletes.h"


MyTableView::MyTableView(QWidget* parent):QTableView(parent){

};

void MyTableView::mouseReleaseEvent(QMouseEvent* e){
    if(e->button() == Qt::LeftButton)
        emit lr(0);
    else if(e->button() == Qt::RightButton)
        emit lr(1);
}



Choice_two_athletes::Choice_two_athletes(QStringList sl, FormMain* parent) : QDialog(parent){
    setFixedSize(800,600);
    p = parent;             // родительский widget
    col = 6;                // количество колонок в таблице
    //фамилии
    lbl_fam_red = new QLabel("<font color=\"red\">Фамилия</font>", this);
    lbl_fam_blue = new QLabel("<font color=\"blue\">Фамилия</font>", this);
    //ввод фамилии
    inFam = new QLineEdit(this);
    inFam->setMaxLength(7);
    l.append("-");
    QFont* f = new QFont();
    f->setPointSize(10);
    inFam->setFont(*f);
    red = "-";
    blue = "-";

    tbl = new MyTableView();    // таблица

    try{
        foreach(QString each, sl)
            l.append(each);
    }catch(...){
        qDebug() << "err each";
    }

    l.sort();
    int num_fam = l.length() ;      //количество фамилий в списке
    int raw = num_fam / col;
    if(num_fam % col)
        raw++;              //количество строк в таблице

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
    hbox->addWidget(lbl_fam_red, 2);
    hbox->addWidget(inFam, 2);
    hbox->addWidget(lbl_fam_blue, 2);

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

    connect(sel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this,
                 SLOT(sel(QItemSelection, QItemSelection)));
    connect(inFam, SIGNAL(textEdited(const QString&)), this, SLOT(textEdited(QString)));

    connect(tbl, SIGNAL(lr(int)), this, SLOT(sel_(int)));

    sel_data = "-";
    b = "";
    r = "";
    setModal(true);
    show();
}

void Choice_two_athletes::showEvent(QShowEvent*){
    inFam->setFocus();
}

void Choice_two_athletes::textEdited(QString x){
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
    connect(tbl->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                                   SLOT(sel(QItemSelection, QItemSelection)));
}

void Choice_two_athletes::closeEvent(QCloseEvent*){
    p->fr2->rate_total_red->setText("");
    p->fr2->rate_total_blue->setText("");

    p->fr2->rate_sum->setText("");
    p->fr2->count_priem->setText("");

    p->fr2->rate_1->setText("");
    p->fr2->rate_2->setText("");
    p->fr2->rate_3->setText("");
    p->fr2->rate_4->setText("");
    p->fr2->rate_5->setText("");
    p->fr2->rate_1->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; "
                                  "border-style: solid; border-color:white;  background-color: black;}");
    p->fr2->rate_2->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    p->fr2->rate_3->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    p->fr2->rate_4->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");
    p->fr2->rate_5->setStyleSheet("QLabel {color: white; border-radius: 20px; border-width: 2px; border-style: "
                                  "solid; border-color:white;  background-color: black;}");

    family_set();

    p->GroupBox_2->setEnabled(true);
    p->RbBlue->setChecked(true);

    if((red != '-') && (blue != '-')){
        p->Btn_next->setEnabled(true);
        p->GroupBox->setEnabled(false);
        p->GroupBox->setEnabled(false);
        p->Cmb_round->setEnabled(false);
        p->CmbAge->setEnabled(false);
        p->CmbWeight->setEnabled(false);
        p->Btn_new->setEnabled(false);
        p->BtnNewFinal->setEnabled(false);
        p->mainwin->menuBar()->setEnabled(false);
     }else{
        p->Btn_next->setEnabled(false);
        p->GroupBox->setEnabled(false);
        p->Cmb_round->setEnabled(true);
        p->CmbAge->setEnabled(true);
        p->CmbWeight->setEnabled(true);
        p->Btn_new->setEnabled(true);
        p->BtnNewFinal->setEnabled(true);
        p->mainwin->menuBar()->setEnabled(true);
        if(p->current_mode == -1)
            p->GroupBox->setEnabled(true);
    }
 }

void Choice_two_athletes::sel(QItemSelection a, QItemSelection){
    sel_data = a.indexes()[0].data().toString();
}

void Choice_two_athletes::sel_(int lr){
    if(lr){
        blue = sel_data;
        if((sel_data != "-") && (sel_data != NULL)){
            lbl_fam_blue->setText("<font color=\"Blue\">" + sel_data.split(';')[0] + "<br>" +
                                                            sel_data.split(';')[1] + "</font>");
        }else{
            lbl_fam_blue->setText("<font color=\"Blue\">" + sel_data + "</font>");
        }
    }else{
        red = sel_data;
        if((sel_data != "-") && (sel_data != NULL)){
            lbl_fam_red->setText("<font color=\"Red\">" + sel_data.split(";")[0] + "<br>" +
                                                          sel_data.split(";")[1] + "</font>");
        }else{
            lbl_fam_red->setText("<font color=\"Red\">" + sel_data + "</font>");
        }
    }
    inFam->setFocus();
}



void Choice_two_athletes::resizeEvent(QResizeEvent*){
        for(int i=0;i<col;i++)
            tbl->setColumnWidth(i,(tbl->rect().width()) / col);
}

QString Choice_two_athletes::sep(QString fam){
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

void Choice_two_athletes::family_set(void){
    p->Lbl_name_red->setText(red);
    if((red != "-") && (red != NULL)){
        QStringList stt = red.split(";");
        QStringList stt2 = stt[0].split(" ");
        p->fr2->name_red->Text(sep(stt2[0].toUpper() + " " + stt2[1]));
        p->fr2->region_red->Text(stt[1]);
        QString path = "flags/";
        path = path + stt[1] + ".png";

        QFile file(path);
        if(file.exists()){
            p->flag_red.load(path);
            p->fr2->lbl_flag_red->setPixmap(p->flag_red.scaled(p->fr2->lbl_flag_red->size()));

        }else
            p->fr2->lbl_flag_red->clear();

    }else{
        p->fr2->name_red->Text("");
        p->fr2->region_red->Text("");
        p->fr2->lbl_flag_red->clear();
        p->Lbl_name_blue->setText(blue);
    }
    p->Lbl_name_blue->setText(blue);
    if((blue != "-") && (blue != NULL)){
        QStringList stt = blue.split(";");
        QStringList stt2 = stt[0].split(" ");
        p->fr2->name_blue->Text(sep(stt2[0].toUpper() + " " + stt2[1]));
        p->fr2->region_blue->Text(stt[1]);
        QString path = "flags/";
        path = path + stt[1] + ".png";
        QFile file(path);
        if(file.exists()){
            p->flag_red.load(path);
            p->fr2->lbl_flag_blue->setPixmap(p->flag_blue.scaled(p->fr2->lbl_flag_blue->size()));

        }else
            p->fr2->lbl_flag_blue->clear();
    }else{
        p->fr2->name_blue->Text("");
        p->fr2->region_blue->Text("");
        p->fr2->lbl_flag_blue->clear();
    }
}
