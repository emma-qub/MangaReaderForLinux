#include "MainWindow.h"
#include "Utils.h"

#include <iostream>

MainWindow::MainWindow(QMainWindow* parent):
  QMainWindow(parent),
  _chaptersDownloaded(0),
  _totalChaptersToDownload(0) {

  QString appIconPath = Utils::getIconsPath()+"/appIcon.png";
  QIcon appIcon(appIconPath);

  _mangaListWidget = new MangaListWidget;
  _mangaReadWidget = new MangaReadWidget;
  _mangaDownloadWidget = new MangaDownloadWidget;

  _tabWidget = new QTabWidget;
  _tabWidget->addTab(_mangaListWidget, QIcon(Utils::getIconsPath()+"/listIcon.png"), "");
  _tabWidget->addTab(_mangaReadWidget, QIcon(Utils::getIconsPath()+"/readIcon.png"), "");
  _tabWidget->addTab(_mangaDownloadWidget, QIcon(Utils::getIconsPath()+"/downloadIcon.png"), "");

  _tabWidget->setIconSize(QSize(60, 80));
  _tabWidget->setTabPosition(QTabWidget::West);
  _tabWidget->setFocusPolicy(Qt::NoFocus);

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
  setWindowIcon(appIcon);

  connect(_mangaListWidget, SIGNAL(chapterSelected(QString,QString)), this, SLOT(switchToRead(QString,QString)));
  connect(_mangaListWidget, SIGNAL(mangaSelected(QString)), this, SLOT(switchToDownload(QString)));

  connect(_mangaDownloadWidget, SIGNAL(chapterSelected(QString, QString)), this, SLOT(switchToRead(QString,QString)));
  connect(_mangaDownloadWidget, SIGNAL(initModelRequested(QString)), _mangaListWidget, SLOT(initModel(QString)));
  connect(_mangaDownloadWidget, SIGNAL(downloading(bool)), _mangaListWidget, SLOT(setDownloadButtonDisabled(bool)));

  connect(_mangaReadWidget, SIGNAL(chapterSelected(QString, QString)), _mangaListWidget, SLOT(updateReadChapter(QString, QString)));

  checkMangaDirectoryExists();
}

void MainWindow::checkMangaDirectoryExists(void) {

}

void MainWindow::switchToRead(QString mangaName, QString chapterName) {
  _tabWidget->setCurrentWidget(_mangaReadWidget);

  _mangaReadWidget->switchManga(mangaName, chapterName);
}

void MainWindow::switchToDownload(QString mangaName) {
  _tabWidget->setCurrentWidget(_mangaDownloadWidget);

  _mangaDownloadWidget->searchForDownload(mangaName);
}
