#include "MainWindow.h"
#include "Utils.h"

MainWindow::MainWindow(QMainWindow* parent):
  QMainWindow(parent) {

  QString appIconPath = Utils::getIconsPath()+"/appIcon.png";
  QIcon appIcon(appIconPath);

  _notificationDialog = new NotificationDialog(appIconPath, this);
  _notificationAnimation = new QPropertyAnimation(_notificationDialog, "geometry");
  connect(_notificationDialog, SIGNAL(hideRequested(void)), this, SLOT(hideNotification(void)));
  connect(_notificationAnimation, SIGNAL(finished(void)), _notificationDialog, SLOT(hide(void)));

  _mangaListWidget = new MangaListWidget;
  _mangaReadWidget = new MangaReadWidget;
  _mangaDownloadWidget = new MangaDownloadWidget;

  _tabWidget = new QTabWidget;
  _tabWidget->addTab(_mangaListWidget, QIcon(Utils::getIconsPath()+"/listIcon.png"), "");
  _tabWidget->addTab(_mangaReadWidget, QIcon(Utils::getIconsPath()+"/readIcon.png"), "");
  _tabWidget->addTab(_mangaDownloadWidget, QIcon(Utils::getIconsPath()+"/downloadIcon.png"), "");

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
  setWindowIcon(appIcon);

  connect(_mangaListWidget, SIGNAL(chapterSelected(QString,QString)), this, SLOT(switchToRead(QString,QString)));
  connect(this, SIGNAL(toReadSwitched(QString,QString)), _mangaReadWidget, SLOT(switchManga(QString,QString)));

  connect(_mangaListWidget, SIGNAL(mangaSelected(QString)), this, SLOT(switchToDownload(QString)));
  connect(this, SIGNAL(toDownloadSwitched(QString)), _mangaDownloadWidget, SLOT(searchForDownload(QString)));

  connect(_mangaDownloadWidget, SIGNAL(initModelRequested()), _mangaListWidget, SLOT(initModel()));
  connect(_mangaDownloadWidget, SIGNAL(downloadDone(QString, QString)), this, SLOT(notifyDownload(QString, QString)));
  connect(_mangaDownloadWidget, SIGNAL(chapterSelected(QString, QString)), this, SLOT(switchToRead(QString,QString)));

  connect(_mangaReadWidget, SIGNAL(chapterSelected(QString, QString)), _mangaListWidget, SLOT(updateReadChapter(QString, QString)));

  checkMangaDirectoryExists();
}

void MainWindow::checkMangaDirectoryExists(void) {

}

void MainWindow::switchToRead(QString mangaName, QString chapterName) {
  _tabWidget->setCurrentWidget(_mangaReadWidget);

  emit toReadSwitched(mangaName, chapterName);
}

void MainWindow::switchToDownload(QString mangaName) {
  _tabWidget->setCurrentWidget(_mangaDownloadWidget);

  emit toDownloadSwitched(mangaName);
}

void MainWindow::notifyDownload(QString title, QString message) {
  _notificationDialog->showPopup(title, message);
}

void MainWindow::hideNotification(void) {
  _notificationAnimation->setDuration(3000);

  _notificationAnimation->setEasingCurve(QEasingCurve::InQuad);

  _notificationDialog->setGeometry(QStyle::alignedRect(Qt::RightToLeft, Qt::AlignBottom, _notificationDialog->size(), qApp->desktop()->availableGeometry()));

  QRect geometryStart = _notificationDialog->geometry();
  QRect geometryEnd = _notificationDialog->geometry();
  geometryEnd.moveTop(geometryEnd.top()+100);
  _notificationAnimation->setStartValue(geometryStart);
  _notificationAnimation->setEndValue(geometryEnd);

  _notificationAnimation->start();
}
