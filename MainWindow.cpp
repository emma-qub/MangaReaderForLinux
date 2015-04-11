#include "MainWindow.h"
#include "Utils.h"

MainWindow::MainWindow() {
    _mangaListWidget = new MangaListWidget;
    _mangaReadWidget = new MangaReadWidget;
    _mangaDownloadWidget = new MangaDownloadWidget;

    _tabWidget = new QTabWidget;
    _tabWidget->addTab(_mangaListWidget, QIcon("../MangaReader/icons/listIcon.png"), "");
    _tabWidget->addTab(_mangaReadWidget, QIcon("../MangaReader/icons/readIcon.png"), "");
    _tabWidget->addTab(_mangaDownloadWidget, QIcon("../MangaReader/icons/downloadIcon.png"), "");

    _tabWidget->setIconSize(QSize(60, 80));
    _tabWidget->setTabPosition(QTabWidget::West);

    QString dockWidgetTitleStyle;
    dockWidgetTitleStyle += "QTabBar::tab {";
    dockWidgetTitleStyle += "    background-color: #ededed;";
    dockWidgetTitleStyle += "}";
    dockWidgetTitleStyle += "QTabWidget::pane {";
    dockWidgetTitleStyle += "    border: solid 1px #000;";
    dockWidgetTitleStyle += "}";
    dockWidgetTitleStyle += "QTabBar::tab:selected {";
    dockWidgetTitleStyle += "    background-color: #313131;";
    dockWidgetTitleStyle += "}";

    setStyleSheet(dockWidgetTitleStyle);

    setCentralWidget(_tabWidget);

    setWindowTitle("Manga Manager");
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon("../icons/appIcon.png"));

    connect(_mangaListWidget, SIGNAL(chapterSelected(QString,QString)), this, SLOT(switchToRead(QString,QString)));
    connect(this, SIGNAL(toReadSwitched(QString,QString)), _mangaReadWidget, SLOT(switchManga(QString,QString)));

    connect(_mangaListWidget, SIGNAL(mangaSelected(QString)), this, SLOT(switchToDownload(QString)));
    connect(this, SIGNAL(toDownloadSwitched(QString)), _mangaDownloadWidget, SLOT(searchForDownload(QString)));

    connect(_mangaDownloadWidget, SIGNAL(downloadDone()), _mangaListWidget, SLOT(initModel()));
    connect(_mangaDownloadWidget, SIGNAL(chapterSelected(QString, QString)), this, SLOT(switchToRead(QString,QString)));

    connect(_mangaReadWidget, SIGNAL(chapterSelected(QString, QString)), _mangaListWidget, SLOT(updateReadChapter(QString, QString)));
}

void MainWindow::switchToRead(QString mangaName, QString chapterName) {
    _tabWidget->setCurrentWidget(_mangaReadWidget);

    emit toReadSwitched(mangaName, chapterName);
}

void MainWindow::switchToDownload(QString mangaName) {
    _tabWidget->setCurrentWidget(_mangaDownloadWidget);

    emit toDownloadSwitched(mangaName);
}


//MGAS 96 avenue de Suffren 75730 Paris cedex 15
//Mme Engevin MGAS au 01 44 10 55 55
//3646 Sécurité Sociale CPAM
