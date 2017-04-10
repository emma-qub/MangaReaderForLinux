#include "MainWindow.h"
#include "Utils.h"
#include "MangaListWidget.h"
#include "MangaReadWidget.h"
#include "SlidingStackedWidget.hxx"
#include "MangaDownloadWidget.h"

#include <QFontDatabase>
#include <QLayout>

#include <QDebug>

MainWindow::MainWindow(QMainWindow* parent):
  QMainWindow(parent) {

  QString appIconPath = Utils::getIconsPath()+"/appIcon.png";
  QIcon appIcon(appIconPath);

  m_mangaListWidget = new MangaListWidget;
  m_mangaReadWidget = new MangaReadWidget;
  m_stackedWidget = new SlidingStackedWidget(this);
  m_stackedWidget->addWidget(m_mangaListWidget);
  m_stackedWidget->addWidget(m_mangaReadWidget);
  m_stackedWidget->setCurrentIndex(eList);
  m_mangaDownloadWidget = new MangaDownloadWidget;

  m_tabWidget = new QTabWidget;
  m_tabWidget->addTab(m_stackedWidget, QIcon(Utils::getIconsPath()+"/readIcon.png"), "");
  m_tabWidget->addTab(m_mangaDownloadWidget, QIcon(Utils::getIconsPath()+"/downloadIcon.png"), "");

  m_tabWidget->setIconSize(QSize(0, 0));
  m_tabWidget->setTabPosition(QTabWidget::West);
  m_tabWidget->setFocusPolicy(Qt::NoFocus);

  setCentralWidget(m_tabWidget);
  layout()->setSpacing(0);
  layout()->setContentsMargins(0, 0, 0, 0);

  setWindowTitle("Manga Manager");
  setWindowState(Qt::WindowMaximized);
  setWindowIcon(appIcon);

  connect(m_mangaListWidget, &MangaListWidget::chapterSelected, this, &MainWindow::switchToRead);
  connect(m_mangaListWidget, &MangaListWidget::downloadMangaRequested, this, &MainWindow::switchToDownload);

  connect(m_mangaReadWidget, &MangaReadWidget::goToListRequested, this, &MainWindow::switchToList);
  connect(m_mangaReadWidget, &MangaReadWidget::currentChapterChanged, m_mangaListWidget, &MangaListWidget::currentChapterChanged);

  connect(m_mangaDownloadWidget, SIGNAL(chapterSelected(QString, QString)), this, SLOT(switchToRead(QString,QString)));
  connect(m_mangaDownloadWidget, SIGNAL(initModelRequested(QString)), m_mangaListWidget, SLOT(initModel(QString)));

  if (QFontDatabase::addApplicationFont("../MangaReaderForLinux/icons/FontAwesome/fonts/FontAwesome.otf") < 0) {
    qWarning() << "FontAwesome cannot be loaded!";
  }
}

void MainWindow::switchToRead(QString const& p_mangaName, QString const& p_chapterName) {
  m_stackedWidget->slideInNext();
  m_mangaReadWidget->switchManga(p_mangaName, p_chapterName);
}

void MainWindow::switchToDownload(const QString& p_mangaName) {
  m_tabWidget->setCurrentWidget(m_mangaDownloadWidget);
  m_mangaDownloadWidget->searchForDownload(p_mangaName);
}

void MainWindow::switchToList()
{
  m_stackedWidget->slideInPrev();
}
