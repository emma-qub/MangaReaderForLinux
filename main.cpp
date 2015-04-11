#include <QApplication>

#include <QDebug>

#include "MainWindow.h"
#include "DownloadManager.h"
#include "PieView.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
/////////////////////////////////////////////////

//    QStandardItemModel* model = new QStandardItemModel(0, 2);
//    model->setHeaderData(0, Qt::Horizontal, "Label");
//    model->setHeaderData(1, Qt::Horizontal, "Quantity");

//    QDir scanDirectory("/home/valentin/Images/Scans");
//    QStringList mangasList = scanDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

//    int row = 0;
//    foreach (const QString& manga, mangasList) {
//        model->insertRow(row, QModelIndex());

//        QDir mangaDirectory(scanDirectory.path()+"/"+manga);
//        QString nbChapters = QString::number(mangaDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size());

//        int r = (16*(row+1))%256;
//        int g = (32*(row+1))%256;
//        int b = (64*(row+1))%256;

//        model->setData(model->index(row, 0, QModelIndex()), manga);
//        model->setData(model->index(row, 1, QModelIndex()), nbChapters);
//        model->setData(model->index(row, 0, QModelIndex()), QColor(r, g, b), Qt::DecorationRole);

//        row++;
//    }

//    PieView* view = new PieView;
////    QTableView* view = new QTableView;
//    view->setModel(model);

////    QHeaderView* headerView = view->horizontalHeader();
////    headerView->setStretchLastSection(true);

//    view->show();


/////////////////////////////////////////////////
    MainWindow window;
    window.show();
/////////////////////////////////////////////////
    return app.exec();
}
