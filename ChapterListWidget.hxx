#ifndef CHAPTERLISTWIDGET_HXX
#define CHAPTERLISTWIDGET_HXX

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QFont>
#include <QApplication>
#include <QFontMetrics>
#include <QMouseEvent>

#include <QDebug>

class FrontCoverOverlay: public QWidget {
  Q_OBJECT

public:
  FrontCoverOverlay(QWidget* p_parent = nullptr):
    QWidget(p_parent),
    m_availableDownloadCount(0),
    m_percentageRead(0),
    m_enteredButton(false),
    m_hasToRepaint(false),
    m_textButtonColor(QColor::fromRgb(0x25, 0x28, 0x38)) {

    auto mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setFixedHeight(300);

    setMouseTracking(true);
  }

  void setFrontCover(QPixmap const& p_frontCover) {
    if (!p_frontCover.isNull()) {
      m_frontCover = p_frontCover.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).copy(0, 50, width(), 300);
    } else {
      m_frontCover = QPixmap(width(), 300);
      m_frontCover.fill(Qt::white);
    }
    repaint();
  }

  void setMangaName(QString const& p_mangaName) {
    m_mangaName = p_mangaName;
    repaint();
  }

  void setAvailableDownloadCount(int p_availableDownloadCount) {
    m_availableDownloadCount = p_availableDownloadCount;
    repaint();
  }

  void setReadProgression(float p_percentageRead) {
    m_percentageRead = p_percentageRead;
    repaint();
  }

protected:
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    QRect drawingRect(0, 0, width(), height());
    painter.drawPixmap(drawingRect, m_frontCover);
    painter.fillRect(drawingRect, QColor::fromRgb(0, 0, 0, 128));
    auto frontCoverFont = QApplication::font();
    frontCoverFont.setWeight(99);
    frontCoverFont.setPixelSize(50);
    painter.setFont(QFont(frontCoverFont));
    painter.setPen(QPen(Qt::white));
    painter.drawText(drawingRect, m_mangaName, QTextOption(Qt::AlignCenter));

    if (m_availableDownloadCount > 0) {
      /// Font
      QFont font;
      font.setFamily("FontAwesome");
      font.setPixelSize(18);
      font.setWeight(99);
      /// Text
      QString downloadText("Update ("+QString::number(m_availableDownloadCount)+") \uf019");
      /// Sizes
      QFontMetrics fm(font);
      int downloadTextWidth = fm.width(downloadText)+60;
      int downloadTextHeight = fm.height()+25;
      m_buttonRect = QRect(width()/2 - downloadTextWidth/2, height()/2+30, downloadTextWidth, downloadTextHeight);
      /// Draw button
      painter.setBrush(QBrush(Qt::white));
      painter.drawRoundedRect(m_buttonRect, 5, 5);
      /// Draw text
      painter.setPen(QPen(m_textButtonColor));
      painter.setFont(font);
      painter.drawText(m_buttonRect, downloadText, QTextOption(Qt::AlignCenter));
    } else {
      m_buttonRect = QRect();
    }

    /// Draw read progression
    auto progressionWidth = (width() * m_percentageRead) / 100;
    auto progressionHeight = 5;
    QRect progresionRect(drawingRect.left(), drawingRect.bottom()-progressionHeight, progressionWidth, progressionHeight);
    auto greenColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
    painter.setBrush(greenColor);
    painter.setPen(QPen(greenColor));
    painter.drawRect(progresionRect);
  }

  void mousePressEvent(QMouseEvent* p_event) override {
    if (m_buttonRect.contains(p_event->pos()) && p_event->button() == Qt::LeftButton) {
      emit downloadRequested();
    }
  }

  void mouseMoveEvent(QMouseEvent* p_event) override {

    if (m_enteredButton != m_buttonRect.contains(p_event->pos())) {
      m_enteredButton = m_buttonRect.contains(p_event->pos());
      m_hasToRepaint = true;
    }

    if (m_hasToRepaint) {
      if (m_enteredButton) {
        m_textButtonColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
      } else {
        m_textButtonColor = QColor::fromRgb(0x25, 0x28, 0x38);
      }
      repaint();
      m_hasToRepaint = false;
    }
  }

signals:
  void downloadRequested();

private:
  int m_availableDownloadCount;
  float m_percentageRead;
  bool m_enteredButton;
  bool m_hasToRepaint;
  QColor m_textButtonColor;
  QString m_mangaName;
  QPixmap m_frontCover;
  QRect m_buttonRect;
};





#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>

#include "Utils.h"

class ChapterListWidget: public QWidget {
  Q_OBJECT

public:
  enum ChapterListColumns {
    eChapterNameColumn = 0,
    eChapterReadColumn
  };
  enum ChapterDataRole {
    eChapterReadRole = Qt::UserRole
  };

  ChapterListWidget(QWidget* p_parent = nullptr);
  void setReadPercentage(int p_chaptersReadCount, int p_allChaptersCount);
  void setAvailableDownloadCount(const QModelIndex& p_index);

public slots:
  void changeManga(const QModelIndex& p_index);
  void markChapterAsRead(QString const& p_chapterName);

signals:
  void chapterSelected(QModelIndex const& p_chapterIndex);
  void downloadRequested();
  void progressionChanged(int p_remainingChaptersToRead);

protected:
  void updateReadState(QStandardItem* p_stateItem, bool p_isChapterRead);
  QModelIndex getChapterIndex(QModelIndex const& p_index);
  void keyReleaseEvent(QKeyEvent* p_event) override;

private:
  void updateChapters();
  void updateTitle();

private:
  int m_chaptersReadCount;
  int m_allChaptersCount;
  QString m_currentMangaName;
  FrontCoverOverlay* m_frontCover;
  QStandardItemModel* m_chaptersModel;
  QTreeView* m_chaptersView;
  QLabel* m_chaptersTitleLabel;
};

#endif // CHAPTERLISTWIDGET_HXX
