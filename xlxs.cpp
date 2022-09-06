#include <QDebug>

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


int readSportsmens(QList<QString>* ql){
    QList<QString>* qL = ql;
    QVector<CellLocation> clList;
    int count = 0;
    int maxRow = -1;
    int maxCol = -1;

    Document doc("sportsmens.xlsx");
    if (!doc.load()){
        qDebug() << "not opened";
        return -1;
    }
    doc.workbook()->setActiveSheet(0);
    Worksheet* wsheet = (Worksheet*)doc.workbook()->activeSheet();
    clList = wsheet->getFullCells( &maxRow, &maxCol );

    for(int row = 0; row < maxRow; row++){
        for(int col = 0; col < maxCol; col++){
            CellLocation cl = clList.at(count++); // cell location
            qL->append(cl.cell.data()->value().toString());

        }
        //qvStr.clear();
    }
    return 0;
}

int readReferees(QList<QString>* ql){
    QList<QString>* qL = ql;
    QVector<CellLocation> clList;
    int count = 0;
    int maxRow = -1;
    int maxCol = -1;

    Document doc("sportsmens.xlsx");
    if (!doc.load()){
        qDebug() << "not opened";
        return -1;
    }
    doc.workbook()->setActiveSheet(1);
    Worksheet* wsheet = (Worksheet*)doc.workbook()->activeSheet();
    clList = wsheet->getFullCells( &maxRow, &maxCol );

    for(int row = 0; row < maxRow; row++){
        for(int col = 0; col < maxCol; col++){
            CellLocation cl = clList.at(count++); // cell location
            qL->append(cl.cell.data()->value().toString());

        }
    }
    return 0;
}
