#ifndef CHAPTERINFOWIDGET_H
#define CHAPTERINFOWIDGET_H

#include <QtWidgets>

class ChapterInfoWidget: public QWidget {
  Q_OBJECT

public:
  explicit ChapterInfoWidget(QWidget* parent = nullptr);

  inline void setNumberFiles(const QString& numberFiles) { m_numberFilesLabel->setText(numberFiles); }
  inline void setTotalSize(const QString& totalSize) { m_totalSizeLabel->setText(totalSize); }
  inline void setLastModification(const QString& lastModification) { m_lastModificationLabel->setText(lastModification); }
  inline void setLastRead(const QString& lastRead) { m_lastReadLabel->setText(lastRead); }
  inline void setImgPreview(const QPixmap& imgPreview) { m_imgPreviewLabel->setPixmap(imgPreview.scaled(m_imgPreviewLabel->width(), m_imgPreviewLabel->height(), Qt::KeepAspectRatio)); }

signals:

public slots:

private:
  QLabel* m_numberFilesLabel;
  QLabel* m_totalSizeLabel;
  QLabel* m_lastModificationLabel;
  QLabel* m_lastReadLabel;
  QLabel* m_imgPreviewLabel;
};

#endif // CHAPTERINFOWIDGET_H
