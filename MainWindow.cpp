#include "MainWindow.h"
#include "Utils.h"


MainWindow::MainWindow(QMainWindow* parent):
  QMainWindow(parent),
  m_chaptersDownloaded(0),
  m_totalChaptersToDownload(0) {

  QString appIconPath = Utils::getIconsPath()+"/appIcon.png";
  QIcon appIcon(appIconPath);

  m_mangaListWidget = new MangaListWidget;
  m_mangaReadWidget = new MangaReadWidget;
  m_stackedWidget = new QStackedWidget;
  m_stackedWidget->addWidget(m_mangaListWidget);
  m_stackedWidget->addWidget(m_mangaReadWidget);
  m_stackedWidget->setCurrentIndex(eList);
  m_mangaDownloadWidget = new MangaDownloadWidget;

  m_tabWidget = new QTabWidget;
  m_tabWidget->addTab(m_stackedWidget, QIcon(Utils::getIconsPath()+"/readIcon.png"), "");
  m_tabWidget->addTab(m_mangaDownloadWidget, QIcon(Utils::getIconsPath()+"/downloadIcon.png"), "");

  ///m_tabWidget->setIconSize(QSize(60, 80));
  m_tabWidget->setIconSize(QSize(0, 0));
  m_tabWidget->setTabPosition(QTabWidget::West);
  m_tabWidget->setFocusPolicy(Qt::NoFocus);

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

  setCentralWidget(m_tabWidget);
  layout()->setSpacing(0);
  layout()->setContentsMargins(0, 0, 0, 0);

  setWindowTitle("Manga Manager");
  setWindowState(Qt::WindowMaximized);
  setWindowIcon(appIcon);

  connect(m_mangaListWidget, SIGNAL(chapterSelected(QString,QString)), this, SLOT(switchToRead(QString,QString)));
  connect(m_mangaListWidget, SIGNAL(mangaSelected(QString)), this, SLOT(switchToDownload(QString)));

  connect(m_mangaReadWidget, &MangaReadWidget::goToListRequested, this, &MainWindow::switchToList);
  connect(m_mangaReadWidget, &MangaReadWidget::currentChapterChanged, m_mangaListWidget, &MangaListWidget::currentChapterChanged);

  connect(m_mangaDownloadWidget, SIGNAL(chapterSelected(QString, QString)), this, SLOT(switchToRead(QString,QString)));
  connect(m_mangaDownloadWidget, SIGNAL(initModelRequested(QString)), m_mangaListWidget, SLOT(initModel(QString)));
  connect(m_mangaDownloadWidget, SIGNAL(downloading(bool)), m_mangaListWidget, SLOT(setDownloadButtonDisabled(bool)));

  checkMangaDirectoryExists();


  QFile file("../MangaReaderForLinux/css/MainWindow.css");
  file.open(QFile::ReadOnly);
  QString newStyleSheet = QLatin1String(file.readAll());
  setStyleSheet(styleSheet() + newStyleSheet);


  if (QFontDatabase::addApplicationFont("../MangaReaderForLinux/icons/FontAwesome/fonts/FontAwesome.otf") < 0) {
    qWarning() << "FontAwesome cannot be loaded!";
  }
}

void MainWindow::checkMangaDirectoryExists() {

}

void MainWindow::switchToRead(QString mangaName, QString chapterName) {
  m_stackedWidget->setCurrentIndex(eRead);

  m_mangaReadWidget->switchManga(mangaName, chapterName);
}

void MainWindow::switchToDownload(QString mangaName) {
  m_tabWidget->setCurrentWidget(m_mangaDownloadWidget);

  m_mangaDownloadWidget->searchForDownload(mangaName);
}

void MainWindow::switchToList()
{
  m_stackedWidget->setCurrentIndex(eList);
}
